#!/usr/bin/env python3
"""Copy an asm file without the functions that have been decompiled to C.

    tools/strip_funcs.py asm/game.s build/decomp/game.s func_A func_B ...

Removes each function's `nonmatching`/`glabel`...`endlabel` block (and the
`.balign` emitted by tools/align_data.py right before it). Refuses if a removed
block contains labels other code may reference (alabel/jlabel/ehlabel): those
need handling first (e.g. a jumptable in rodata pointing into the function).
"""
import re
import sys
from pathlib import Path


def main() -> None:
    src, dst, *funcs = sys.argv[1:]
    text = Path(src).read_text()
    for name in funcs:
        pat = re.compile(
            rf"(?:^\.balign \d+\n)?^nonmatching {re.escape(name)}, \w+\n\n"
            rf"^glabel {re.escape(name)}\n.*?^endlabel {re.escape(name)}\n",
            re.M | re.S,
        )
        m = pat.search(text)
        if not m:
            sys.exit(f"strip_funcs: {name} not found in {src}")
        inner = re.findall(r"^\s*(alabel|jlabel|ehlabel) (\S+)", m.group(0), re.M)
        if inner:
            sys.exit(f"strip_funcs: {name} has labels referenced from elsewhere: {inner}")
        text = text[: m.start()] + text[m.end() :]
    Path(dst).parent.mkdir(parents=True, exist_ok=True)
    Path(dst).write_text(text)


if __name__ == "__main__":
    main()
