#!/usr/bin/env python3
"""story_flag_names.py - the story flags by name in the rooms' scripts.

The names are scripts/story-names.fs's constants (`$222 constant front-garden-4-camomile`). A
literal story flag before `story-flag?`, `story-flag-set` or `story-flag-clear` becomes its name,
as does one handed to Hewie's sniffing (`$19 576 var-set`: the flag his find sets; `$1A`: the one
taking it sets).

    tools/story_flag_names.py      (scripts/story/rooms/*.fs, scripts/story/shared.fs)
"""
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
FILES = [*sorted((ROOT / "scripts/story/rooms").glob("*.fs")), ROOT / "scripts/story/shared.fs"]
NUM = r"(\$[0-9A-Fa-f]+|\d+)"
FLAG = re.compile(r"(?<!\S)" + NUM + r"(\s+story-flag(?:\?|-set|-clear))(?!\S)")
SNIFF = re.compile(r"(?<!\S)(\$1[9Aa]\s+)" + NUM + r"(\s+var-set)(?!\S)")


def number(t):
    return int(t[1:], 16) if t.startswith("$") else int(t)


def names():
    out = {}
    for line in (ROOT / "scripts/story-names.fs").read_text().split("\n"):
        m = re.match(r"\$([0-9A-Fa-f]+)\s+constant\s+(\S+)", line)
        if m:
            out.setdefault(int(m.group(1), 16), m.group(2))
    return out


def using(text):
    """story-names in the file's USING: list"""
    return re.sub(r"^(USING:(?:(?!story-names)[^;\n])*?)( ;)", r"\1 story-names\2", text, count=1, flags=re.M)


def main():
    n = names()
    total = 0
    for f in FILES:
        s = f.read_text()
        t, a = FLAG.subn(lambda m: (n.get(number(m.group(1)), m.group(1))) + m.group(2), s)
        t, b = SNIFF.subn(lambda m: m.group(1) + n.get(number(m.group(2)), m.group(2)) + m.group(3), t)
        if t != s:
            f.write_text(using(t))
            total += a + b
    print(f"{total} story flags named")


if __name__ == "__main__":
    main()
