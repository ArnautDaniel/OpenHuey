#!/usr/bin/env python3
"""Name a symbol in config/symbol_addrs.txt (replacing auto-generated entries at the same address).

    tools/name.py Scene_Update 0x0011F9C0 func ["Scene vtable +0xC"]
    tools/name.py sGameStateMain 0x00414370          # data: no type

An existing func_XXXXXXXX / D_XXXXXXXX / L_XXXXXXXX line for the address is
replaced; a hand-written name at the address is an error (rename it explicitly).
Comments must not contain ':' (splat parses `key:value` attributes in them).
Re-run `configure.py --split` afterwards.
"""
import re
import sys
from pathlib import Path

SYM = Path("config/symbol_addrs.txt")
AUTO = re.compile(r"^(?:func|D|L)_[0-9A-F]{8}$")


def main() -> None:
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    name, addr = sys.argv[1], int(sys.argv[2], 0)
    kind = sys.argv[3] if len(sys.argv) > 3 else None
    note = sys.argv[4] if len(sys.argv) > 4 else None
    if note and ":" in note:
        sys.exit("comments must not contain ':'")
    lines = SYM.read_text().splitlines()
    out, replaced = [], False
    for line in lines:
        m = re.match(r"^(\w+) = 0x([0-9A-Fa-f]+);", line)
        if m and int(m.group(2), 16) == addr:
            if m.group(1) == name:
                replaced = True
                continue  # rewritten below
            if not AUTO.match(m.group(1)):
                sys.exit(f"0x{addr:08X} is already named {m.group(1)}")
            replaced = True
            continue
        if m and m.group(1) == name:
            sys.exit(f"{name} already used for 0x{int(m.group(2), 16):08X}")
        out.append(line)
    entry = f"{name} = 0x{addr:08X};"
    if kind:
        entry += f" // type:{kind}"
        if note:
            entry += f"  ({note})"
    elif note:
        entry += f" // {note}"
    out.append(entry)
    SYM.write_text("\n".join(out) + "\n")
    print(("renamed" if replaced else "added") + f": {entry}")


if __name__ == "__main__":
    main()
