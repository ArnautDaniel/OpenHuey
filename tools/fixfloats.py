#!/usr/bin/env python3
"""Rewrite decimal float literals in C files as exact hex floats.

    tools/fixfloats.py src/foo.c [...]      (in place; prints what changed)

The ps2dev GCC (r5900 single-float format) rounds decimal float literals toward
zero: 0.6f becomes 0x3F199999, while the original Metrowerks build has the
correctly rounded 0x3F19999A. An exactly representable hex literal is taken as is,
so every `1.5f`-style literal that is not exact is replaced by its round-to-nearest
float32 value in hex (the decimal is kept in a comment).
"""
import re
import struct
import sys
from pathlib import Path

LIT = re.compile(r"(?<![\w.])((?:\d+\.\d*|\.\d+)(?:[eE][+-]?\d+)?|\d+[eE][+-]?\d+)[fF]\b")


def f32(x: float) -> float:
    return struct.unpack("<f", struct.pack("<f", x))[0]


def fix(text: str) -> tuple[str, int]:
    n = 0

    def repl(m: re.Match) -> str:
        nonlocal n
        v = float(m.group(1))
        r = f32(v)
        if r == v:  # exact already: GCC gets it right
            return m.group(0)
        n += 1
        return f"{r.hex()}f /* {m.group(1)} */"

    out = []
    for line in text.splitlines(keepends=True):
        code, sep, comment = line.partition("//")
        # don't touch literals inside /* */ comments on the same line (rare; keep it simple)
        parts = re.split(r"(/\*.*?\*/)", code)
        parts = [p if p.startswith("/*") else LIT.sub(repl, p) for p in parts]
        out.append("".join(parts) + sep + comment)
    return "".join(out), n


def main() -> None:
    for name in sys.argv[1:]:
        p = Path(name)
        text, n = fix(p.read_text())
        if n:
            p.write_text(text)
            print(f"{name}: {n} literal(s)")


if __name__ == "__main__":
    main()
