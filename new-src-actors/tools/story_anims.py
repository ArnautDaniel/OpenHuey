#!/usr/bin/env python3
"""story_anims.py - the rooms' animations and sounds by name (run once over the converted scripts).

A script's character is known from the room's own calls: `who act action` / `action-force`, and
Fiona's action scripts offered on her button (`act when offer-action`); the shared scripts the
same, from any room. In a script whose character is known, the animation it plays itself
(`self-anim`, `self-anim-blend`, `self-anim-blend8`) gets that character's name for it
(fiona/anims.fs, hewie/anims.fs, stalker/anims.fs); in one whose character isn't known, only
Fiona's story animations are named (nobody else has those ids). A character's sound from its
own bank (bank 5: `fiona-who $43 5 char-sound`) gets its name (fiona/sounds.fs, hewie/sounds.fs).
A name more than one of these vocabularies has is written with its vocabulary
(`fiona.anims:stand-anim`).

    tools/story_anims.py
"""
import collections
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
ROOMS = sorted((ROOT / "scripts/story/rooms").glob("*.fs"))
SHARED = ROOT / "scripts/story/shared.fs"
VOCABS = {"fiona.anims": "fiona/anims.fs", "hewie.anims": "hewie/anims.fs", "stalker.anims": "stalker/anims.fs",
          "fiona.sounds": "fiona/sounds.fs", "hewie.sounds": "hewie/sounds.fs", "stalker.sounds": "stalker/sounds.fs"}
WHO = {"fiona-who": "fiona", "hewie-who": "hewie", "stalker-who": "stalker"}
NUM = r"(\$[0-9A-Fa-f]+|\d+)"


def number(t):
    return int(t[1:], 16) if t.startswith("$") else int(t)


def table(vocab):
    out = {}
    for line in (ROOT / "scripts" / VOCABS[vocab]).read_text().split("\n"):
        for t, name in re.findall(r"(?<!\S)(\$?[0-9A-Fa-f]+) constant (\S+)", line.split("\\")[0]):
            if "(bank 6)" not in line:
                out.setdefault(number(t), name)
    return out


TABLES = {v: table(v) for v in VOCABS}
COUNT = collections.Counter(n for t in TABLES.values() for n in t.values())
STORY = {v for v, n in TABLES["fiona.anims"].items() if v >= 0x800 and (0x800 <= v < 0x803 or 0x900 <= v < 0x906 or 0xA00 <= v < 0xA03
         or 0xF00 <= v < 0xF05 or v in (0x1002, 0x1003) or 0x1D00 <= v < 0x1D03)}


def code(text):
    return re.sub(r"\\[^\n]*", "", text)


def owners():
    """{(file stem or 'shared', act): {character}}"""
    own = collections.defaultdict(set)
    for f in [*ROOMS, SHARED]:
        c = code(f.read_text())
        for who, act in re.findall(r"(?<!\S)\S+\s+(\S+)\s+" + NUM + r"\s+action(?:-force)?(?!\S)", c):
            if who in WHO:
                v = number(act)
                own[("shared" if v >= 0x80 else f.stem, v & 0xFF)].add(WHO[who])
        for act, _ in re.findall(r"(?<!\S)" + NUM + r"\s+(\S+)\s+offer-action(?!\S)", c):
            v = number(act)
            own[("shared" if v >= 0x80 else f.stem, v & 0xFF)].add("fiona")
    return own


def name_of(vocab, v):
    n = TABLES[vocab].get(v)
    if n is None:
        return None
    return f"{vocab}:{n}" if COUNT[n] > 1 else n


ANIM = re.compile(r"(?<!\S)" + NUM + r"(\s+(?:\S+\s+)?self-anim(?:-blend8|-blend)?)(?!\S)")
SOUND = re.compile(r"(?<!\S)(fiona-who|hewie-who)(\s+)" + NUM + r"(\s+5\s+char-sound)(?!\S)")


def convert(f, own, stem):
    text = f.read_text()
    used = set()
    out, last = [], 0
    for m in re.finditer(r"^: \S+\.(act([0-9A-F]{2})|\S+) .*?^;", text, re.M | re.S):
        body = m.group(0)
        who = own.get((stem, int(m.group(2), 16))) if m.group(2) else None
        who = next(iter(who)) if who and len(who) == 1 else None

        def anim(a):
            v = number(a.group(1))
            vocab = f"{who}.anims" if who else "fiona.anims"
            if not who and v not in STORY:
                return a.group(0)
            n = name_of(vocab, v)
            if n is None:
                return a.group(0)
            used.add(vocab)
            return n + a.group(2)

        def sound(s):
            vocab = WHO[s.group(1)] + ".sounds"
            n = name_of(vocab, number(s.group(3)))
            if n is None:
                return s.group(0)
            used.add(vocab)
            return s.group(1) + s.group(2) + n + s.group(4)

        new = SOUND.sub(sound, ANIM.sub(anim, body))
        out.append(text[last:m.start()] + new)
        last = m.end()
    out.append(text[last:])
    t = "".join(out)
    for v in sorted(used):
        if not re.search(r"^USING:[^\n]*(?<!\S)" + re.escape(v) + r"(?!\S)", t, re.M):
            t = re.sub(r"^(USING:[^;\n]*?)( ;)", r"\1 " + v + r"\2", t, count=1, flags=re.M)
    if t != text:
        f.write_text(t)
    return sum(1 for _ in ANIM.finditer(code(text))) - sum(1 for _ in ANIM.finditer(code(t)))


def main():
    own = owners()
    n = sum(convert(f, own, f.stem) for f in ROOMS) + convert(SHARED, own, "shared")
    print(f"animations named; {n} numeric self-anim calls fewer")


if __name__ == "__main__":
    main()
