#!/usr/bin/env python3
"""Debug aid: insert EE SIO console markers at function entries (edits asm/ in place;
re-run `configure.py --split` to undo).

    tools/marker.py func_0020D860=M FlushCache=F ...

Each marker writes "#<char>\\n" to the SIO TX register (0x1000F180), which PCSX2
logs as EE console output. Uses $k0/$k1 only.
"""
import re
import sys
from pathlib import Path


def snippet(ch: str) -> str:
    out = ["    .set noat", "    lui $k0, 0x1001"]
    for c in "#" + ch + "\n":
        out += [f"    addiu $k1, $zero, {ord(c)}", "    sb $k1, -0xE80($k0)"]
    return "\n".join(out) + "\n"


def main() -> None:
    marks = dict(a.split("=", 1) for a in sys.argv[1:])
    for p in Path("asm").glob("*.s"):
        text = p.read_text()
        for name, ch in list(marks.items()):
            pat = re.compile(rf"^(glabel {re.escape(name)}\n)", re.M)
            text, n = pat.subn(lambda m: m.group(1) + snippet(ch), text)
            if n:
                marks.pop(name)
                print(f"marked {name} with #{ch} in {p}")
        p.write_text(text)
    if marks:
        sys.exit(f"not found: {', '.join(marks)}")


if __name__ == "__main__":
    main()
