#!/usr/bin/env python3
"""Rename symbols everywhere: config/symbol_addrs.txt, the C (src/, include/, native/), the
difftest list and the docs. Then run `configure.py --split` (the asm takes the new names).

    tools/rename.py LIST          # LIST: lines "old new [func|data] [comment]"
    tools/rename.py old new ...   # one rename

`old` is a func_XXXXXXXX / D_XXXXXXXX name or a name already in symbol_addrs.txt (its
address is kept); a name that isn't a symbol (a static helper, a macro) is only replaced in
the sources. Whole words only. Files are rewritten once for the whole list.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SYM = ROOT / "config/symbol_addrs.txt"
TEXT_GLOBS = ["src/**/*.c", "src/**/*.h", "src/**/*.inc", "include/**/*.h", "native/**/*.c", "native/**/*.h",
              "native/**/*.S", "native/**/*.py", "tools/difftest_list.txt", "docs/**/*.md"]
AUTO = re.compile(r"^(func|D)_([0-9A-F]{8})$")


def load_list(argv):
    items = []
    if len(argv) == 1:
        for line in Path(argv[0]).read_text().splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split(None, 3)
            items.append(parts)
    else:
        items.append(argv)
    return items


def main() -> None:
    if not sys.argv[1:]:
        sys.exit(__doc__)
    items = load_list(sys.argv[1:])
    sym_lines = SYM.read_text().splitlines()
    by_name = {}
    for i, line in enumerate(sym_lines):
        m = re.match(r"^(\w+) = 0x([0-9A-Fa-f]+);", line)
        if m:
            by_name[m.group(1)] = i
    names_in_use = set(by_name)
    mapping = {}
    for it in items:
        old, new = it[0], it[1]
        kind = it[2] if len(it) > 2 and it[2] in ("func", "data") else None
        note = it[3] if len(it) > 3 else (it[2] if len(it) > 2 and kind is None else None)
        if note and (":" in note or ";" in note):
            sys.exit(f"{old}: comments must not contain ':' or ';'")
        if new in names_in_use or new in mapping.values():
            sys.exit(f"{new} is already used")
        mapping[old] = new
        if old in by_name:
            line = sym_lines[by_name[old]]
            sym_lines[by_name[old]] = re.sub(r"^\w+", new, line)
            names_in_use.add(new)
            continue
        m = AUTO.match(old)
        if m:
            addr = int(m.group(2), 16)
            if kind is None:
                kind = "func" if m.group(1) == "func" else None
            entry = f"{new} = 0x{addr:08X};"
            if kind == "func":
                entry += " // type:func"
                if note:
                    entry += f"  ({note})"
            elif note:
                entry += f" // {note}"
            sym_lines.append(entry)
            names_in_use.add(new)
    SYM.write_text("\n".join(sym_lines) + "\n")
    word = re.compile(r"\b(" + "|".join(map(re.escape, sorted(mapping, key=len, reverse=True))) + r")\b")
    changed = 0
    for g in TEXT_GLOBS:
        for p in ROOT.glob(g):
            if not p.is_file():
                continue
            t = p.read_text(errors="surrogateescape")
            n = word.sub(lambda m: mapping[m.group(1)], t)
            if n != t:
                p.write_text(n, errors="surrogateescape")
                changed += 1
    print(f"{len(mapping)} renamed, {changed} files rewritten; now run configure.py --split")


if __name__ == "__main__":
    main()
