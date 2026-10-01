#!/usr/bin/env python3
"""Apply config/pointers.txt to splat's data asm (run by configure.py after splat).

spimdisasm only symbolizes a data word when its value hits a known symbol, so
pointers into the middle of functions/objects stay raw, and the odd coincidental
value gets symbolized when it shouldn't. Each line of config/pointers.txt fixes
one 4-byte word, addressed by vram:

    0x003EADF4            # pointer: becomes `.word <containing label> + off`
    0x003EA99C = _end     # pointer with an explicit expression
    0x003A1C18 raw        # NOT a pointer: force the plain number

`#` starts a comment.
"""
import re
import struct
import sys
from bisect import bisect_right
from pathlib import Path

BASEROM = "baserom/SLUS_210.75"
FILE_OFF, VRAM = 0x80, 0x00100000
LINE_RE = re.compile(r"^(\s*/\* [0-9A-F]+ ([0-9A-F]{8})(?: [0-9A-F]{8})? \*/ )(\.\w+)\s+(.*)$")
LABEL_RE = re.compile(r"^\s*(?:glabel|alabel|dlabel|jlabel|ehlabel)\s+(\S+)")
VRAM_RE = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8})")


def load_entries(path: Path) -> dict[int, str | None]:
    """addr -> expression, None for 'auto', '' for raw."""
    out: dict[int, str | None] = {}
    for n, line in enumerate(path.read_text().splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        m = re.fullmatch(r"(0x[0-9A-Fa-f]+)(?:\s*=\s*(\S.*)|\s+(raw))?", line)
        if not m:
            sys.exit(f"{path}:{n}: can't parse {line!r}")
        addr = int(m.group(1), 16)
        out[addr] = "" if m.group(3) else m.group(2)
    return out


def collect_labels(files: list[Path]) -> tuple[list[int], list[str]]:
    """Global labels with their vram, from every asm file."""
    labels: dict[int, str] = {}
    for p in files:
        pending: list[str] = []
        for line in p.read_text().splitlines():
            m = LABEL_RE.match(line)
            if m:
                pending.append(m.group(1))
                continue
            if pending:
                v = VRAM_RE.search(line)
                if v:
                    a = int(v.group(1), 16)
                    for name in pending:
                        # prefer a function/object name over alias labels
                        labels.setdefault(a, name)
                    pending = []
    addrs = sorted(labels)
    return addrs, [labels[a] for a in addrs]


SIZES = {".byte": 1, ".short": 2}


def eval_c_string(lit: str) -> bytes:
    """Decode the body of a gas string literal ("..." already stripped of quotes)."""
    return lit.strip('"').encode("latin1").decode("unicode_escape").encode("latin1")


def merge_small(lines: list[str], i: int, a: int, prefix: str, expr: str, p: Path) -> None:
    """Replace the .byte/.short lines covering [a, a+4) with one .word.

    Labels that fall inside the word are re-defined relative to its start.
    """
    covered, j, inner = 0, i, []
    while covered < 4:
        line = lines[j]
        m = LINE_RE.match(line)
        if m:
            size = SIZES.get(m.group(3))
            if size is None or covered + size > 4:
                sys.exit(f"0x{a:08X}: can't merge {m.group(3)} into a word in {p}")
            covered += size
            lines[j] = ""
        else:
            lm = re.match(r"\s*(dlabel|nonmatching|enddlabel)\s+(\S+)", line)
            if lm and lm.group(1) == "dlabel":
                inner.append((lm.group(2), covered))
            if line.strip() and not lm:
                sys.exit(f"0x{a:08X}: unexpected line while merging in {p}: {line!r}")
            if lm and lm.group(1) != "enddlabel":
                lines[j] = ""
        j += 1
    tmp = f".Lptr_{a:08X}"
    out = [f"{tmp}:\n", f"{prefix}.word {expr}\n"]
    for name, off in inner:
        out.append(f"    .globl {name}\n    .type {name}, @object\n    .set {name}, {tmp} + {off}\n")
    lines[i] = "".join(out)


def main() -> None:
    entries = load_entries(Path("config/pointers.txt"))
    auto = Path("config/pointers_auto.txt")
    if auto.exists():
        for a, e in load_entries(auto).items():
            entries.setdefault(a, e)  # hand-curated entries win
    if not entries:
        return
    rom = Path(BASEROM).read_bytes()
    asm_files = sorted(Path("asm").rglob("*.s"))
    addrs, names = collect_labels(asm_files)

    def expr_for(value: int) -> str:
        i = bisect_right(addrs, value) - 1
        if i < 0:
            sys.exit(f"no label at or below 0x{value:08X}")
        off = value - addrs[i]
        return names[i] if off == 0 else f"{names[i]} + 0x{off:X}"

    todo = dict(entries)
    for p in Path("asm").rglob("*.s"):
        if "data" not in p.parts:
            continue
        lines = p.read_text().splitlines(keepends=True)
        changed = False
        for i, line in enumerate(lines):
            m = LINE_RE.match(line)
            if not m:
                continue
            a = int(m.group(2), 16)
            if a not in todo:
                continue
            (value,) = struct.unpack_from("<I", rom, a - VRAM + FILE_OFF)
            e = todo.pop(a)
            new = f"0x{value:08X}" if e == "" else (e or expr_for(value))
            if m.group(3) in (".word", ".float"):
                lines[i] = f"{m.group(1)}.word {new}\n"
            elif m.group(3) == ".asciz" and len(eval_c_string(m.group(4))) == 3:
                lines[i] = f"{m.group(1)}.word {new}\n"  # pointer mis-decoded as "xyz\0"
            else:
                merge_small(lines, i, a, m.group(1), new, p)
            changed = True
        if changed:
            p.write_text("".join(lines))
    if todo:
        sys.exit("pointers.txt entries not found in data asm: " + ", ".join(f"0x{a:08X}" for a in todo))
    print(f"ptrpatch: applied {len(entries)} entries")


if __name__ == "__main__":
    main()
