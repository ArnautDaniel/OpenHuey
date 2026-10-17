#!/usr/bin/env python3
"""story_rename.py - rename story words across the scripts that use them.

The rooms' scripts were converted from the game's bytecode with the converter's names (opcodes,
offsets). This renames words wholesale - their definitions and every use - in scripts/story/,
the tests and docs/subsystems/story.md. A Forth word is whitespace-delimited, so a name is
replaced only where it stands alone (or after a vocabulary's `story.words:`).

    tools/story_rename.py old=new [old=new ...]
    tools/story_rename.py -f renames.txt      (lines "old new", # comments)
"""
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
FILES = [*sorted((ROOT / "scripts/story").rglob("*.fs")), *sorted((ROOT / "tests").rglob("*.fs")),
         ROOT / "docs/subsystems/story.md"]


def pairs(args):
    if args[:1] == ["-f"]:
        for line in open(args[1]):
            line = line.split("#", 1)[0].split()
            if line:
                yield line[0], line[1]
        return
    for a in args:
        old, new = a.split("=", 1)
        yield old, new


def main(args):
    renames = dict(pairs(args))
    pat = re.compile(r"(?<![^\s:])(" + "|".join(re.escape(o) for o in sorted(renames, key=len, reverse=True)) + r")(?!\S)")
    # (in the docs, words are written in backquotes)
    doc = re.compile(r"`(" + "|".join(re.escape(o) for o in renames) + r")([ `])")
    total = 0
    for f in FILES:
        s = f.read_text()
        if f.suffix == ".md":
            t, n = doc.subn(lambda m: "`" + renames[m.group(1)] + m.group(2), s)
        else:
            t, n = pat.subn(lambda m: renames[m.group(1)], s)
        if n:
            f.write_text(t)
            total += n
            print(f"{f.relative_to(ROOT)}: {n}")
    print(f"{total} renamed")


if __name__ == "__main__":
    main(sys.argv[1:])
