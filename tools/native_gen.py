#!/usr/bin/env python3
"""Generate the inputs of the native (x86, 32-bit) build from the PS2 disassembly.

    tools/native_gen.py [-o build/native] [--windows]

--windows: for 32-bit Windows (MinGW): C symbols get a leading underscore there, and COFF
objects have no .type / .size.
--strip: leave the game's own data out of the build (zero-filled space in its place): hg then
copies it from SLUS_210.75 on the player's disc at startup (native/platform/slus.c). Either way
each data file gets a table for that copy (where it starts, its offset in SLUS_210.75, its
length, and the parts to keep: relocations - pointers into the native program - and the space
left for labels written in C).

- slus.c: the list of those tables, and the size and FNV-1a hash of the SLUS_210.75 they fit.
- data/*.S: asm/data/*.s rewritten for the host assembler. Same symbols, same layout (32-bit
  build), and every pointer is a relocation, so tables, vtables and PTMFs point at the native
  functions. Jump tables into MIPS code (`.word .Lxxxx`) become 0: C code has its own switches.
- stubs.c: a weak stub for every function of the PS2 executable. A function written in C (or a
  platform replacement) is a strong definition and wins at link time; reaching a stub reports
  "undecompiled func_XXXXXXXX" (native/platform/stub.c), which says what to decompile next.
"""
import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

MACROS = """\
.macro dlabel label, visibility=global
    .\\visibility \\label
    .type \\label, @object
\\label:
.endm
.macro enddlabel label
    .size \\label, . - \\label
.endm
.macro nonmatching label, size=1
.endm
.macro jlabel label, visibility=global
\\label:
.endm
"""

# 32-bit Windows (--windows): C names are "_name" in the object files; COFF has no .type / .size
WIN = False
MACROS_WIN = """\
.macro dlabel label, visibility=global
    .\\visibility _\\label
_\\label:
.endm
.macro enddlabel label
.endm
.macro nonmatching label, size=1
.endm
.macro jlabel label, visibility=global
_\\label:
.endm
"""


def sym(arg):
    """a data operand with its symbols as the object files name them"""
    return re.sub(r"(?<![\w.$])([A-Za-z_]\w*)", r"_\1", arg) if WIN else arg


DIRECTIVE = re.compile(r"^(\s*(?:/\*.*?\*/)?\s*)\.(word|align)\b\s*(.*)$")
SECTION = re.compile(r'^\.section\s+(\.\w+)')


# labels written as C (native/data/group1.c, tools/data_c.py): left as zero-filled space here
GROUP1 = ROOT / "native/data/group1.txt"


def group1():
    if not GROUP1.exists():
        return set()
    return {l.strip() for l in GROUP1.read_text().splitlines() if l.strip() and not l.startswith("#")}


NUMBER = re.compile(r"^-?(0x[0-9A-Fa-f]+|[0-9]+)$")
# a line of data: (file offset, address: none on a string's continuation lines), directive, operand
CONTENT = re.compile(r"^\s*(?:/\* ([0-9A-F]+) ([0-9A-F]{8})(?: [0-9A-F]+)? \*/)?\s*\.(word|float|short|byte|ascii|asciz|space)\s+(.*)$")
SIZES = {"word": 4, "float": 4, "short": 2, "byte": 1}


def gas_string(arg):
    """the bytes of a GNU as string literal (as .ascii puts it)"""
    s = arg.strip()
    assert s[0] == '"' and s[-1] == '"', arg
    s, out, i = s[1:-1], 0, 0
    while i < len(s):
        if s[i] != "\\":
            out, i = out + 1, i + 1
            continue
        i += 1
        if s[i] in "xX":
            i += 1
            while i < len(s) and s[i] in "0123456789abcdefABCDEF":
                i += 1
        elif s[i] in "01234567":
            j = i
            while i < len(s) and i < j + 3 and s[i] in "01234567":
                i += 1
        else:
            i += 1
        out += 1
    return out


def ident(stem):
    return "hgslus_" + re.sub(r"\W", "_", stem)


def convert(src: Path, skip=frozenset(), strip=False) -> str:
    """The data file for the host assembler, and its table's name (None for .bss). Each line is
    placed at its PS2 offset from the file's first: the gaps (the PS2's alignment) are written as
    space, and each label's offset is checked by the assembler."""
    out = [MACROS_WIN if WIN else MACROS]
    hidden = None   # inside a label that the C defines
    name = ident(src.stem)
    start = None    # the first content line's (file offset, address)
    at = 0          # bytes emitted since then
    relocs = []
    check = False   # a label was just defined: check its offset on the next content line
    label_at = None   # where in `out` that label's line is: a gap before its first content goes before it
    bss = False
    for line in src.read_text().splitlines():
        if line.startswith(".include"):
            continue
        m = re.match(r"(dlabel|nonmatching) (\S+)", line)
        if m and m.group(1) == "dlabel":
            hidden = m.group(2) if m.group(2) in skip else None   # (not every label has an enddlabel)
            check = True
            label_at = len(out)
        if m and m.group(2) in skip:
            continue
        if line.startswith("enddlabel"):
            if line.split()[-1] in skip:   # its symbol is the C's
                hidden = None
                continue
            hidden = None
        m = SECTION.match(line)
        if m:
            bss = m.group(1) == ".bss"
            # writable: the PS2 "rodata" split also holds .sdata/.sbss variables
            if WIN:
                out.append(".bss" if bss else ".data")
            else:
                out.append('.section .bss,"aw",@nobits' if bss else '.section .data,"aw"')
            # same position modulo 128 as on the PS2, so offsets within the file keep their alignment
            first = int(re.search(r"/\* (?:[0-9A-F]+ )?([0-9A-F]{8})(?: [0-9A-F]+)? \*/", src.read_text()).group(1), 16)
            out.append(f".balign 128\n.space {first % 128}")
            continue
        c = CONTENT.match(line)
        if bss or c is None:
            if c is None and start is not None and not bss and DIRECTIVE.match(line) and DIRECTIVE.match(line).group(2) == "align":
                continue   # (the gaps are written from the addresses)
            if c is None and start is not None and not bss and re.match(r"^\s*\.balign\b", line):
                continue
            m = DIRECTIVE.match(line)
            if m and m.group(2) == "word":
                pre, d, arg = m.groups()
                line = f"{pre}.long {sym('0' if arg.startswith('.L') else arg)}"
            elif m:
                line = f"{m.group(1)}.p2align {m.group(3)}"
            out.append(line)
            continue
        if c.group(2) is not None:
            addr = int(c.group(2), 16)
            if start is None:
                start = (int(c.group(1), 16), addr)
                out.append(f"{sym(name)}:")
            gap = addr - start[1] - at
            if gap < 0:
                raise SystemExit(f"{src.name}: 0x{c.group(2)} overlaps the line before")
            if gap:   # (the PS2's alignment: before a label just defined, so the label is its data's)
                if check and label_at is not None:
                    out.insert(label_at, f"    .space {gap}")
                else:
                    out.append(f"    .space {gap}")
                at += gap
            if check:   # (the sizes below are the assembler's too; the label is checked, not just the line)
                i = label_at + (1 if gap else 0) if label_at is not None else len(out)
                lbl = re.match(r"dlabel (\S+)", out[i]) if i < len(out) else None
                here = sym(lbl.group(1)) if lbl and lbl.group(1) not in skip else "."
                out.append(f'.ifne ({here} - {sym(name)}) - {at}\n.error "{src.name}: 0x{c.group(2)} is not at its PS2 offset"\n.endif')
                check = False
                label_at = None
        elif start is None:
            raise SystemExit(f"{src.name}: data before the first address")
        pre = line[:line.index("*/") + 2] if "*/" in line else "   "
        d, arg = c.group(3), c.group(4).strip()
        if d in SIZES:
            n = SIZES[d]
        elif d in ("ascii", "asciz"):
            n = gas_string(arg) + (d == "asciz")
        elif d == "space":
            n = int(arg, 0)
        else:
            raise SystemExit(f"{src.name}: can't size .{d}")
        at += n
        if hidden is not None:   # a label of group1.c: its space stays zero
            relocs.append((f".Lhgr{len(relocs)}", n))
            out += [f"{relocs[-1][0]}:", f"    .space {n}"]
        elif d == "word" and not NUMBER.match(arg):   # a relocation: the native address
            relocs.append((f".Lhgr{len(relocs)}", 4))
            out += [f"{relocs[-1][0]}:", f"{pre} .long {sym('0' if arg.startswith('.L') else arg)}"]
        elif strip and d != "space":
            out.append(f"{pre} .space {n}")
        elif d == "word":
            out.append(f"{pre} .long {arg}")
        else:
            out.append(line)
    if start is not None:
        # the table: start, offset in SLUS_210.75, length, number of parts to keep, each part's
        # offset and size (relocations; the labels group1.c defines, left zero)
        out.append(".Lhgend:")
        out.append(f".globl {sym(name + '_tab')}")
        out.append(".p2align 2")
        out.append(f"{sym(name + '_tab')}:")
        out.append(f"    .long {sym(name)}, {start[0]}, .Lhgend - {sym(name)}, {len(relocs)}")
        for r, n in relocs:
            out.append(f"    .long {r} - {sym(name)}, {n}")
        out.append(f".globl {sym(name)}")
    if not WIN:
        out.append('.section .note.GNU-stack,"",@progbits')
    return "\n".join(out) + "\n", (name if start is not None else None)


THUNK = re.compile(r"^\s+/\* \w+ \w+ \w+ \*/\s+(\S+)\s*(.*)$")


def thunks():
    """Functions that are only `j target` with `addiu $a0, $a0, imm` (or a nop) in the delay slot:
    C++ this-adjusting thunks and tail-call aliases. -> (name, target, adjust)"""
    out = []
    for fn in ("asm/game.s", "asm/sinit.s"):
        name, ins = None, []
        for line in (ROOT / fn).read_text().splitlines():
            m = re.match(r"^\s*glabel (\S+)", line)
            if m:
                name, ins = m.group(1), []
                continue
            if line.startswith("endlabel"):
                if name and len(ins) == 2 and ins[0][0] == "j" and not ins[0][1].startswith("."):
                    op, args = ins[1]
                    a = [x.strip() for x in args.split(",")]
                    if op == "nop":
                        out.append((name, ins[0][1], 0))
                    elif op == "addiu" and a[0] == "$a0" and a[1] == "$a0":
                        out.append((name, ins[0][1], int(a[2], 0)))
                name = None
                continue
            m = THUNK.match(line)
            if m and name:
                ins.append((m.group(1), m.group(2).strip()))
    return out


def functions():
    """(name, address) of every function and alternate entry in the PS2 executable."""
    lab = re.compile(r"^\s*(?:glabel|alabel)\s+(\S+)")
    addr = re.compile(r"^\s+/\* \w+ (\w+) \w+ \*/")
    for fn in ("asm/game.s", "asm/sinit.s"):
        name = None
        for line in (ROOT / fn).read_text().splitlines():
            m = lab.match(line)
            if m:
                name = m.group(1)
                continue
            if name:
                m = addr.match(line)
                if m:
                    yield name, int(m.group(1), 16)
                    name = None


def write(dst, text):
    if not dst.exists() or dst.read_text() != text:
        dst.write_text(text)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-o", "--out", type=Path, default=ROOT / "build/native")
    ap.add_argument("--windows", action="store_true", help="for 32-bit Windows (MinGW)")
    ap.add_argument("--strip", action="store_true", help="the game's data from the disc at startup")
    a = ap.parse_args()
    global WIN
    WIN = a.windows
    (a.out / "data").mkdir(parents=True, exist_ok=True)
    skip = group1()
    tables = []
    for src in sorted((ROOT / "asm/data").glob("*.s")):
        if src.name == "elf_header.s":
            continue
        dst = a.out / "data" / (src.stem + ".S")
        text, tab = convert(src, skip, a.strip)
        if tab:
            tables.append(tab)
        write(dst, text)

    # the tables and the executable they fit
    exe = (ROOT / "baserom/SLUS_210.75").read_bytes()
    h = 0xCBF29CE484222325
    for b in exe:
        h = ((h ^ b) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    lines = ["/* generated by tools/native_gen.py: the game's data in SLUS_210.75 (native/platform/slus.c) */",
             "#include <stdint.h>", ""]
    lines += [f"extern const uint32_t {t}_tab[];" for t in tables]
    lines += ["", "const uint32_t *const hg_slus_tables[] = {"] + [f"    {t}_tab," for t in tables] + ["    0,", "};"]
    lines += [f"const uint32_t hg_slus_size = {len(exe)}u;", f"const uint64_t hg_slus_fnv = 0x{h:016X}ull;",
              f"const int hg_slus_stripped = {int(a.strip)};"]
    write(a.out / "slus.c", "\n".join(lines) + "\n")

    # x86 versions of the thunks (cdecl: `this` is the first stack argument)
    th = thunks()
    asm = ["/* generated by tools/native_gen.py: PS2 this-adjusting thunks / tail-call aliases */", ".text"]
    for name, target, adj in th:
        if WIN:
            # each in its own section: COFF keeps a jump to a weak symbol of the same object as
            # "section + offset", which stays added on when C overrides the target thunk
            asm += [f'.section .text${name},"x"', f".weak _{name}", f"_{name}:"]
        else:
            asm += [f".weak {name}", f".type {name}, @function", f"{name}:"]
        if adj:
            asm.append(f"    addl ${adj}, 4(%esp)")
        asm.append(f"    jmp {sym(target)}")
    if not WIN:
        asm.append('.section .note.GNU-stack,"",@progbits')
    text = "\n".join(asm) + "\n"
    dst = a.out / "thunks.S"
    if not dst.exists() or dst.read_text() != text:
        dst.write_text(text)
    thunk_names = {t[0] for t in th}

    seen = set(thunk_names)
    lines = ["/* generated by tools/native_gen.py */",
             "extern int hg_undecompiled(const char *name, unsigned addr);", ""]
    for name, addr in functions():
        if name in seen or not re.fullmatch(r"[A-Za-z_]\w*", name):
            continue
        seen.add(name)
        lines.append(f'__attribute__((weak)) int {name}(void) {{ return hg_undecompiled("{name}", 0x{addr:08X}u); }}')
    # symbols the data or the C refer to that the disassembly doesn't define (addresses inside
    # crt0 / code, kernel memory): weak placeholders, 256 zero bytes for data
    defined = set(seen)
    refs = set()
    for src in (ROOT / "asm/data").glob("*.s"):
        t = src.read_text()
        defined |= set(re.findall(r"^dlabel (\S+)", t, re.M))
        refs |= set(re.findall(r"\.word ([A-Za-z_]\w*)", t))
    defined |= skip
    for src in (ROOT / "src").rglob("*.c"):
        refs |= set(re.findall(r"\b((?:func|D)_[0-9A-F]{8})\b", src.read_text()))
    for name in sorted(refs - defined):
        if name.startswith("func_"):
            lines.append(f'__attribute__((weak)) int {name}(void) {{ return hg_undecompiled("{name}", 0x{name[5:]}u); }}')
        else:
            lines.append(f"__attribute__((weak)) unsigned char {name}[256];")
    text = "\n".join(lines) + "\n"
    dst = a.out / "stubs.c"
    if not dst.exists() or dst.read_text() != text:
        dst.write_text(text)
    print(f"{len(seen) - len(thunk_names)} stubs, {len(thunk_names)} thunks")


if __name__ == "__main__":
    main()
