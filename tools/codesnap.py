#!/usr/bin/env python3
"""Snapshot every C function's compiled code, to prove a cleanup changed nothing.

    tools/codesnap.py snap DIR           # write DIR/ps2.json and DIR/native.json
    tools/codesnap.py compare OLD NEW    # list functions whose code changed

PS2: GCC's assembly as the ninja build leaves it (build/src/**/*.c.o.s - run ninja first).
Native: the host compiler with the PC build's flags (-S, no -g), run here (3 at a time).

Each function's assembly is normalized so that moving code between files, reordering it,
renaming symbols or renumbering labels doesn't count as a change:
  - local labels (.L12) are numbered in the order the function uses them;
  - constants (.LC4) and jump tables (labels defined outside the function) are replaced by
    their contents;
  - symbols are mapped through config/symbol_addrs.txt + config/renames.txt to their address
    (func_XXXXXXXX / D_XXXXXXXX), so a renamed function or global still compares equal;
  - GCC's numbered suffixes of local statics (kPi.3) are dropped.
Functions are keyed by their canonical name; same-named statics in several files compare as
a sorted list.
"""
import concurrent.futures
import hashlib
import json
import re
import shlex
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
NATIVE_FLAGS = "build/native/cmake/CMakeFiles/hg.dir/flags.make"


def load_canon() -> dict:
    """name -> canonical name (its address form) for every named symbol."""
    canon = {}
    sym = ROOT / "config/symbol_addrs.txt"
    for line in sym.read_text().splitlines():
        m = re.match(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);(.*)", line)
        if not m:
            continue
        name, addr, rest = m.group(1), int(m.group(2), 16), m.group(3)
        is_func = "type:func" in rest or name.startswith("func_")
        canon[name] = ("func_%08X" if is_func else "D_%08X") % addr
    ren = ROOT / "config/renames.txt"   # old static / helper names -> new (cleanup renames)
    if ren.exists():
        for line in ren.read_text().splitlines():
            line = line.split("#")[0].strip()
            if line:
                old, new = line.split()
                canon[new] = canon.get(old, old)
    return canon


IDENT = re.compile(r"[A-Za-z_$][\w$]*(?:\.\d+)?")
LOCAL = re.compile(r"\.L\w+")


def parse(text: str, canon: dict) -> dict:
    """function name -> normalized body text."""
    lines = text.splitlines()
    # labels and what follows them (for constants / jump tables defined outside functions)
    labels = {}
    cur = None
    for ln in lines:
        s = ln.strip()
        m = re.match(r"^(\.L\w+):", s)
        if m:
            cur = m.group(1)
            labels[cur] = []
            continue
        if re.match(r"^[A-Za-z_$][\w$.]*:", s) or s.startswith((".section", ".text", ".data", ".previous")):
            cur = None
            continue
        if cur is not None and s.startswith((".word", ".long", ".dword", ".quad", ".byte", ".half",
                                            ".short", ".ascii", ".string", ".float", ".double",
                                            ".zero", ".space", ".gpword", ".4byte", ".8byte", ".value")):
            labels[cur].append(s)
    funcs = {}
    i = 0
    n = len(lines)
    while i < n:
        s = lines[i].strip()
        m = re.match(r"^\.type\s+([\w$.]+),\s*@function", s)
        if not m:
            i += 1
            continue
        name = m.group(1)
        # find "name:"
        j = i + 1
        while j < n and lines[j].strip() != name + ":":
            j += 1
        body = []
        k = j + 1
        while k < n:
            t = lines[k].strip()
            if t.startswith(".end\t" + name) or t == ".end " + name or re.match(r"^\.size\s+" + re.escape(name) + r",", t):
                break
            body.append(t)
            k += 1
        funcs.setdefault(name, []).append(body)
        i = k + 1
    # data objects (".type x, @object" ... "x:" then its data): compared by contents too
    i = 0
    while i < n:
        m = re.match(r"^\.type\s+([\w$.]+),\s*@object", lines[i].strip())
        i += 1
        if not m:
            continue
        name = m.group(1)
        j = i
        while j < n and lines[j].strip() != name + ":":
            if re.match(r"^\.type\s", lines[j].strip()):
                break
            j += 1
        if j >= n or lines[j].strip() != name + ":":
            continue
        body = []
        k = j + 1
        while k < n:
            t = lines[k].strip()
            if re.match(r"^[A-Za-z_$.][\w$.]*:", t) or t.startswith((".section", ".text", ".data", ".bss",
                                                                       ".rdata", ".previous", ".type", ".globl",
                                                                       ".local", ".comm", ".align", ".p2align")):
                break
            body.append(t)
            k += 1
        funcs.setdefault("obj " + name, []).append(body)
    out = {}
    for name, bodies in funcs.items():
        normed = []
        for body in bodies:
            normed.append(normalize(body, labels, canon))
        if name.startswith("obj "):
            key = "obj " + canon_name(name[4:], canon)
        else:
            key = canon_name(name, canon)
        out.setdefault(key, []).extend(normed)
    return out


def canon_name(name: str, canon: dict) -> str:
    base = re.sub(r"\.\d+$", "", name)
    return canon.get(base, base)


def normalize(body: list, labels: dict, canon: dict) -> str:
    own = set()
    for t in body:
        m = re.match(r"^(\.L\w+):", t)
        if m:
            own.add(m.group(1))
    order = {}

    def lab(l: str) -> str:
        if l not in order:
            order[l] = ".L_%d" % len(order)
        return order[l]

    def sym(m) -> str:
        w = m.group(0)
        if w.startswith(".L"):
            return w
        return canon_name(w, canon)

    def fix(t: str, depth: int = 0) -> str:
        t = re.sub(r"\s+#.*$", "", t)   # comments
        def rep_local(m):
            l = m.group(0)
            if l in own or l not in labels or depth > 2:
                return lab(l)
            # data defined outside: inline it
            return "{" + ";".join(fix(x, depth + 1) for x in labels[l]) + "}"
        t = LOCAL.sub(rep_local, t)
        t = IDENT.sub(sym, t)
        return re.sub(r"\s+", " ", t)

    keep = []
    for t in body:
        if not t or t.startswith(("#", ".loc", ".cfi", ".file", ".p2align", ".align", ".frame", ".mask",
                                  ".fmask", ".set", ".ent", ".type", ".globl", ".local", ".size")):
            continue
        keep.append(fix(t))
    return "\n".join(keep)


def ps2_snap(canon: dict) -> dict:
    out = {}
    for s in sorted((ROOT / "build/src").rglob("*.c.o.s")):
        add(out, parse(s.read_text(errors="replace"), canon))
    return out


def native_cmd() -> list:
    txt = (ROOT / NATIVE_FLAGS).read_text()
    get = lambda k: re.search(r"^" + k + r" = (.*)$", txt, re.M).group(1)
    flags = shlex.split(get("C_DEFINES")) + shlex.split(get("C_INCLUDES")) + shlex.split(get("C_FLAGS"))
    flags = [f for f in flags if f != "-g"]
    return ["cc", "-S", "-o", "-"] + flags


def native_one(args):
    cmd, src = args
    r = subprocess.run(cmd + [str(src)], capture_output=True, text=True, cwd=ROOT)
    if r.returncode != 0:
        sys.exit(f"native compile failed: {src}\n{r.stderr[:2000]}")
    return r.stdout


def native_snap(canon: dict) -> dict:
    cmd = native_cmd()
    srcs = sorted(p for d in ("src/game", "src/leaf", "src/sdk") for p in (ROOT / d).glob("*.c"))
    out = {}
    with concurrent.futures.ThreadPoolExecutor(3) as ex:
        for text in ex.map(native_one, [(cmd, s) for s in srcs]):
            add(out, parse(text, canon))
    return out


def add(out: dict, funcs: dict) -> None:
    for name, bodies in funcs.items():
        out.setdefault(name, []).extend(bodies)


SP_SLOT = re.compile(r"(-?\d+)\((\$sp|\$fp|%esp|%ebp)\)")
SP_ADDR = re.compile(r"(addiu \$\d+,\$sp,)(-?\d+)|(leal )(-?\d+)(\(%esp\))")


# registers whose choice carries no ABI meaning, renamed within their class in order of use:
# MIPS callee-saved / plain temporaries (EABI args are $4-$11, results $2/$3; floats: args
# $f12-$f19, result $f0); x86 callee-saved (cdecl args are on the stack)
REG_CLASSES = [
    ["$%d" % r for r in (16, 17, 18, 19, 20, 21, 22, 23)] + ["$fp"],
    ["$%d" % r for r in (12, 13, 14, 15, 24, 25)],
    ["$f%d" % r for r in range(1, 12)],
    ["$f%d" % r for r in range(20, 32)],
]
X86_FAMILIES = {"ebx": ["%ebx", "%bx", "%bl", "%bh"], "esi": ["%esi", "%si"], "edi": ["%edi", "%di"],
                "ebp": ["%ebp", "%bp"]}
SAVE_RE = re.compile(r"^(sd|sq|ld|lq|sw|lw|swc1|lwc1|sdc1|ldc1) (\$(?:1[6-9]|2[0-3]|fp|31|f2\d|f3[01])),-?\d+\(\$sp\)$"
                     r"|^(pushl|popl) (%ebx|%esi|%edi|%ebp)$")
REG_TOKEN = re.compile(r"\$(?:f\d+|\d+|fp)\b|%[a-z]+")


def loose(body: str) -> str:
    """the body with its stack slots and its free register choices numbered in the order used;
    the saves / restores of callee-saved registers become one line (the set saved)"""
    saved_set = set()
    kept = []
    for t in body.split("\n"):
        m = SAVE_RE.match(t)
        if m:
            saved_set.add(m.group(2) or m.group(4))
            continue
        kept.append(t)
    body = "\n".join(kept)
    order = {}

    def slot(off: str) -> str:
        if off not in order:
            order[off] = "S%d" % len(order)
        return order[off]

    body = SP_SLOT.sub(lambda m: slot(m.group(1)) + "(" + m.group(2) + ")", body)
    body = SP_ADDR.sub(lambda m: (m.group(1) + slot(m.group(2))) if m.group(1) else (m.group(3) + slot(m.group(4)) + m.group(5)), body)
    cls = {}
    for i, c in enumerate(REG_CLASSES):
        for r in c:
            cls[r] = ("m%d" % i, r)
    for fam, regs in X86_FAMILIES.items():
        for k, r in enumerate(regs):
            cls[r] = ("x", fam, k)
    seen = {}

    def reg(m) -> str:
        r = m.group(0)
        c = cls.get(r)
        if c is None:
            return r
        if c[0] == "x":
            fam, k = c[1], c[2]
            key = ("x", fam)
            if key not in seen:
                seen[key] = "R%d" % sum(1 for q in seen if q[0] == "x")
            return seen[key] + "." + str(k)
        key = (c[0], r)
        if key not in seen:
            seen[key] = c[0] + "_%d" % sum(1 for q in seen if q[0] == c[0])
        return seen[key]

    body = REG_TOKEN.sub(reg, body)
    saves = sorted(REG_TOKEN.sub(reg, r) for r in saved_set)
    return body + "\nsaves " + ",".join(saves)


def digest(snap: dict, f=lambda b: b) -> dict:
    return {k: sorted(hashlib.sha1(f(b).encode()).hexdigest() for b in v) for k, v in snap.items()}


def main() -> None:
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    if sys.argv[1] == "snap":
        d = Path(sys.argv[2])
        d.mkdir(parents=True, exist_ok=True)
        canon = load_canon()
        for kind, fn in (("ps2", ps2_snap), ("native", native_snap)):
            snap = fn(canon)
            (d / f"{kind}.json").write_text(json.dumps({"digest": digest(snap), "loose": digest(snap, loose),
                                                        "text": snap}))
            print(f"{kind}: {len(snap)} functions")
    elif sys.argv[1] == "compare":
        bad = 0
        for kind in ("ps2", "native"):
            a = json.loads((Path(sys.argv[2]) / f"{kind}.json").read_text())
            b = json.loads((Path(sys.argv[3]) / f"{kind}.json").read_text())
            da, db = a["digest"], b["digest"]
            gone = sorted(set(da) - set(db))
            new = sorted(set(db) - set(da))
            la, lb = a["loose"], b["loose"]
            both = set(da) & set(db)
            changed = sorted(k for k in both if la[k] != lb[k])
            slots = sorted(k for k in both if da[k] != db[k] and la[k] == lb[k])
            for k in gone:
                print(f"{kind}: gone {k}")
            for k in new:
                print(f"{kind}: new {k}")
            for k in changed:
                print(f"{kind}: changed {k}")
            bad += len(gone) + len(new) + len(changed)
            print(f"{kind}: {len(da)} -> {len(db)} functions, {len(changed)} changed, {len(gone)} gone, "
                  f"{len(new)} new ({len(slots)} differ only in stack slots: {' '.join(slots[:8])})")
        sys.exit(1 if bad else 0)
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
