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


def digest(snap: dict) -> dict:
    return {k: sorted(hashlib.sha1(b.encode()).hexdigest() for b in v) for k, v in snap.items()}


def main() -> None:
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    if sys.argv[1] == "snap":
        d = Path(sys.argv[2])
        d.mkdir(parents=True, exist_ok=True)
        canon = load_canon()
        for kind, fn in (("ps2", ps2_snap), ("native", native_snap)):
            snap = fn(canon)
            (d / f"{kind}.json").write_text(json.dumps({"digest": digest(snap), "text": snap}))
            print(f"{kind}: {len(snap)} functions")
    elif sys.argv[1] == "compare":
        bad = 0
        for kind in ("ps2", "native"):
            a = json.loads((Path(sys.argv[2]) / f"{kind}.json").read_text())
            b = json.loads((Path(sys.argv[3]) / f"{kind}.json").read_text())
            da, db = a["digest"], b["digest"]
            gone = sorted(set(da) - set(db))
            new = sorted(set(db) - set(da))
            changed = sorted(k for k in set(da) & set(db) if da[k] != db[k])
            for k in gone:
                print(f"{kind}: gone {k}")
            for k in new:
                print(f"{kind}: new {k}")
            for k in changed:
                print(f"{kind}: changed {k}")
            bad += len(gone) + len(new) + len(changed)
            print(f"{kind}: {len(da)} -> {len(db)} functions, {len(changed)} changed, {len(gone)} gone, {len(new)} new")
        sys.exit(1 if bad else 0)
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
