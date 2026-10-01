#!/usr/bin/env python3
"""De-symbolize struct-field offsets that spimdisasm mistook for addresses.

Run by configure.py after splat. Pattern (MW codegen for a large offset):

    lui   $v1, %hi(D_014E8FBC)
    addu  $v1, $s0, $v1          # $s0 = pointer to a big object
    sw    $zero, %lo(D_014E8FBC)($v1)

0x014E8FBC is a field offset into the object $s0 points to, not an address.
If it stays a symbol it moves when BSS moves, so the access lands on the
wrong field. The same shape with an *index* register (sll/mult result) is
real array indexing and keeps its symbol.

Only the strict shape lui -> addu(pointer base) -> %lo memory op is rewritten,
and never for a symbol that is also accessed directly somewhere (a real global). config/offsets_keep.txt lists symbols to leave alone.
Writes build/offpatch_report.txt.
"""
import re
from collections import defaultdict
from pathlib import Path

INS = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+([\w.]+)\s*(.*)$")
INDEX_OPS = {
    "sll", "dsll", "dsll32", "sra", "srl", "dsra", "dsrl", "mflo", "mfhi", "mult", "multu",
    "addu", "subu", "daddu", "dsubu", "addiu", "daddiu", "ori", "andi", "slt", "sltu", "and", "or",
}
NO_DEST = {
    "sw", "sh", "sb", "sd", "sq", "swc1", "sdc1", "swl", "swr", "sdl", "sdr", "jr", "jalr",
    "beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez", "beql", "bnel", "j", "jal",
    "b", "mtc1", "mtc0", "ctc1", "sync", "sync.p", "nop", "div", "divu", "mult", "multu",
    "mtlo", "mthi", "syscall", "break", "cache", "pref", "qmtc2",
}
SYM_ADDR = re.compile(r"(?:_|^\.L)([0-9A-F]{8})$")
ADDR_NAMED = ("D_", ".L", "func_", "L_")  # auto-named symbols whose name is their address


PTR_OPS = {"arg", "lw", "lwu", "ld", "lq"}  # base came from a pointer load or an argument


def scan_function(lines: list[str]) -> tuple[dict[int, str], dict[str, set[str]]]:
    """Return (line indices to de-symbolize, symbol -> base kinds seen)."""
    defs: dict[str, str] = {}
    his: dict[str, tuple[str, list[int]]] = {}  # reg -> (sym, lines that built it)
    derived: dict[str, tuple[str, list[int]]] = {}  # reg = base + offset, pending %lo use
    kinds: dict[str, set[str]] = defaultdict(set)
    patch: dict[int, str] = {}  # line -> symbol
    for i, line in enumerate(lines):
        m = INS.search(line)
        if not m:
            continue
        op, args = m.group(2), m.group(3)
        a = [x.strip() for x in args.split(",")]
        ml = re.search(r"%lo\(([\w.]+)\)\((\$\w+)\)", args)
        if ml and ml.group(2) in his and his[ml.group(2)] == (ml.group(1), his[ml.group(2)][1]) and len(his[ml.group(2)][1]) == 1:
            kinds[ml.group(1)].add("direct")  # lui %hi(sym); op %lo(sym)(same reg): a real global
        if ml and ml.group(2) in derived and derived[ml.group(2)][0] == ml.group(1):
            sym, src = derived[ml.group(2)]
            for j in src + [i]:
                patch[j] = sym
        mh = re.search(r"%hi\(([\w.]+)\)", args)
        if op == "lui" and mh:
            his[a[0]] = (mh.group(1), [i])
            defs[a[0]] = "lui"
            derived.pop(a[0], None)
            continue
        mlo = re.search(r"%lo\(([\w.]+)\)", args)
        if op in ("addiu", "daddiu") and mlo and len(a) == 3 and a[1] in his and his[a[1]][0] == mlo.group(1):
            his[a[0]] = (mlo.group(1), his[a[1]][1] + [i])
            defs[a[0]] = "lui"
            continue
        if op in ("addu", "daddu") and len(a) == 3:
            d, x, y = a
            if "$zero" in (x, y):  # register move
                src = x if y == "$zero" else y
                defs[d] = defs.get(src, "arg")
                for t in (his, derived):
                    if src in t:
                        t[d] = t[src]
                    else:
                        t.pop(d, None)
                continue
            hit = False
            for hreg, base in ((x, y), (y, x)):
                if hreg in his and hreg != base:
                    sym, src = his[hreg]
                    if defs.get(base, "arg") in PTR_OPS:
                        kinds[sym].add("ptr")
                        if len(src) == 1:  # lui+addiu forms an array base: leave alone
                            derived[d] = (sym, src)
                    else:
                        kinds[sym].add("index")
                    hit = True
                    break
            if hit:
                defs[d] = "lw"  # base + offset is still a pointer
                his.pop(d, None)
                continue
        if op in NO_DEST or not a or not a[0].startswith("$"):
            continue
        # Pointer-ness survives pointer arithmetic: ptr + imm and ptr + x stay pointers.
        if op in ("addiu", "daddiu") and len(a) == 3 and a[1] != "$zero":
            defs[a[0]] = "lw" if (a[1] == "$sp" or defs.get(a[1], "arg") in PTR_OPS) else op
        elif op in ("addu", "daddu") and len(a) == 3:
            ptrish = any(r == "$sp" or defs.get(r, "arg") in PTR_OPS for r in a[1:] if r != "$zero")
            defs[a[0]] = "lw" if ptrish else op
        else:
            defs[a[0]] = op
        his.pop(a[0], None)
        if not ml:
            derived.pop(a[0], None)
    return patch, kinds


def value_of(name: str, addr_of: dict[str, int]) -> int | None:
    if name in addr_of:
        return addr_of[name]
    m = SYM_ADDR.search(name)
    return int(m.group(1), 16) if m else None


def main() -> None:
    keep_path = Path("config/offsets_keep.txt")
    keep = set()
    if keep_path.exists():
        keep = {l.split("#")[0].strip() for l in keep_path.read_text().splitlines()} - {""}
    report = []
    total = 0
    files = sorted(Path("asm").glob("*.s"))
    texts = {p: p.read_text().split("\n") for p in files}
    funcs = []
    direct: set[str] = set()
    for p, text in texts.items():
        starts = [i for i, l in enumerate(text) if l.startswith("glabel ")] + [len(text)]
        for s, e in zip(starts, starts[1:]):
            patch, kinds = scan_function(text[s:e])
            direct |= {k for k, v in kinds.items() if "direct" in v}
            funcs.append((p, s, text[s].split()[1], patch, kinds))
    changed = set()
    for p, s, func, patch, kinds in funcs:
        text = texts[p]
        for sym, k in kinds.items():
            if "ptr" in k and ("index" in k or sym in direct):
                report.append(f"KEPT {sym} in {func} (also used as {'index' if 'index' in k else 'direct global'})")
        # A symbol that is never a direct global and never indexed here is an
        # offset everywhere in this function (incl. luis in delay slots that
        # flow across branches, which the linear scan can't follow).
        syms = {sym for sym in patch.values()}
        for sym in sorted(syms):
            if sym in direct or sym in keep or not sym.startswith(ADDR_NAMED) or "index" in kinds[sym]:
                continue
            v = value_of(sym, {})
            if v is None:
                continue
            end = next((j for j in range(s + 1, len(text)) if text[j].startswith("glabel ")), len(text))
            for i in range(s, end):
                if f"({sym})" in text[i]:
                    text[i] = text[i].replace(f"%hi({sym})", f"%hi(0x{v:X})").replace(f"%lo({sym})", f"%lo(0x{v:X})")
                    total += 1
                    changed.add(p)
            report.append(f"offset {sym} in {func}")
    for p in changed:
        p.write_text("\n".join(texts[p]))
    Path("build").mkdir(exist_ok=True)
    Path("build/offpatch_report.txt").write_text("\n".join(report) + "\n")
    print(f"offpatch: rewrote {total} operands ({sum(r.startswith('offset') for r in report)} symbol uses)")


if __name__ == "__main__":
    main()
