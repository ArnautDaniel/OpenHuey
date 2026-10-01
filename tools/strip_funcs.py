#!/usr/bin/env python3
"""Copy an asm file without the functions that have been decompiled to C.

    tools/strip_funcs.py asm/game.s build/decomp/game.s func_A func_B ...

Removes each function's `nonmatching`/`glabel`...`endlabel` block (and the
`.balign` emitted by tools/align_data.py right before it). Jump table labels (jlabel) are kept, so the now-unused table in rodata still
links. Refuses if a removed block has alternate entry points (alabel/ehlabel).
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
        inner = re.findall(r"^\s*(alabel|ehlabel) (\S+)", m.group(0), re.M)
        if inner:
            sys.exit(f"strip_funcs: {name} has labels referenced from elsewhere: {inner}")
        # Jump table targets: the table in rodata is unused once the C replaces the
        # function, but its entries must still resolve, so keep the labels (all at the
        # place the function was).
        keep = "".join(f"  jlabel {lbl}\n" for lbl in re.findall(r"^\s*jlabel (\S+)", m.group(0), re.M))
        text = text[: m.start()] + keep + text[m.end() :]
    Path(dst).parent.mkdir(parents=True, exist_ok=True)
    Path(dst).write_text(text)


if __name__ == "__main__":
    main()
