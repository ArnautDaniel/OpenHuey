#!/usr/bin/env python3
"""story_room_names.py - each room's own event bits, script variables and sounds by name.

A room's event bits (0..31: `ebit?`, `ebit-set`, `ebit-clear`), script variables (0..$18:
`var-set`, `var?`, `var`, `var-inc`, `var-dec`, ...) and sounds (bank 6, loaded for it) mean
something only in that room. The names are in a table - `room kind number name | comment` (kind:
bit, var or snd; a sound's flag bits stay as words, `kept-going` and `volume-scaled`) - and this
puts them in each room's file: a block of constants after its USING: line (with the comments), and every
literal use of one replaced by its name. A name the room already sees (a story word, an item, a
flag) or one used twice in a room gets a suffix.

    tools/story_room_names.py names.txt [more.txt ...]
"""
import collections
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
ROOMS = ROOT / "scripts/story/rooms"
NUM = r"(\$[0-9A-Fa-f]+|\d+)"
VISIBLE = ["story/words.fs", "story/shared.fs", "story-names.fs", "flag-names.fs", "floor-flags.fs",
           "room-names.fs", "fiona/anims.fs", "hewie/anims.fs", "stalker/anims.fs", "fiona/sounds.fs",
           "hewie/sounds.fs", "items/names.fs", "messages.fs", "engine.fs"]


def number(t):
    return int(t[1:], 16) if t.startswith("$") else int(t)


def visible_words():
    out = set()
    for f in VISIBLE:
        p = ROOT / "scripts" / f
        if p.exists():
            out |= set(re.findall(r"(?:^|\s)(?::|constant|variable|create|message|fvariable|enum:[^;]*?)\s+(\S+)", p.read_text()))
    return out


def suffixed(name, kind):
    if kind == "snd":
        return name[:-len("-sound")] + "-room-sound"
    return name + ("-bit" if kind == "bit" else "-local")


FLAGS = ((0x80000000, "kept-going"), (0x40000000, "volume-scaled"))


def sound_id(sounds, t):
    """A bank-6 sound's id by name, with its flag bits as words; or as it was."""
    n = number(t)
    name = sounds.get(n & 0xFFFF)
    if name is None or n & ~0xC000FFFF:
        return t
    return " ".join([name] + [w for bit, w in FLAGS if n & bit])


def table(paths):
    rooms = collections.defaultdict(dict)
    for p in paths:
        for line in open(p):
            if not line.strip() or line.startswith("#"):
                continue
            head, _, comment = line.partition("|")
            room, kind, n, name = head.split()[:4] if len(head.split()) >= 4 else ([*head.split()[:1], "bit", *head.split()[1:3]])
            rooms[room][(kind, int(n))] = (name, comment.strip())
    return rooms


def main(args):
    taken = visible_words()
    rooms = table(args)
    for room, names in sorted(rooms.items()):
        f = ROOMS / f"{room}.fs"
        text = f.read_text(encoding="latin-1")
        # the room's own words, and names unique within it
        own = set(re.findall(r"^: (\S+)", text, re.M))
        used, final = collections.Counter(), {}
        for key in sorted(names):
            name, comment = names[key]
            base = name if name not in taken and name not in own else suffixed(name, key[0])
            used[base] += 1
            final[key] = (base if used[base] == 1 else f"{base}-{used[base]}", comment)
        bits = {n: v[0] for (k, n), v in final.items() if k == "bit"}
        vars_ = {n: v[0] for (k, n), v in final.items() if k == "var"}

        sounds = {n: v[0] for (k, n), v in final.items() if k == "snd"}

        def snd(m):
            return sound_id(sounds, m.group(1)) + m.group(2)

        def bit(m):
            return bits.get(number(m.group(1)), m.group(1)) + m.group(2)

        def var2(m):
            return vars_.get(number(m.group(1)), m.group(1)) + m.group(2)

        lines = text.split("\n")
        out = []
        for l in lines:
            code, sep, com = l.partition(" \\ ") if not l.lstrip().startswith("\\") else ("", "", l)
            if l.lstrip().startswith("\\"):
                out.append(l)
                continue
            code = re.sub(r"(?<!\S)" + NUM + r"(\s+ebit(?:\?|-set|-clear))(?!\S)", bit, code)
            code = re.sub(r"(?<!\S)" + NUM + r"(\s+\S+\s+(?:var-set|var\?))(?!\S)", var2, code)
            code = re.sub(r"(?<!\S)" + NUM + r"(\s+(?:var|var-inc|var-dec|thousandths|effect-out|zone-point))(?!\S)", var2, code)
            code = re.sub(r"(?<!\S)" + NUM + r"(\s+(?:6|\$86)\s+(?:char-sound|sound-stop|(?:\S+\s+){5}sound))(?!\S)", snd, code)
            out.append(code + sep + com)
        text = "\n".join(out)
        # the block after USING: (or added to the one already there)
        heading = "\\ ---- this room's own event bits, script variables and sounds ----"
        text = text.replace("\\ ---- this room's own event bits and script variables ----", heading)
        block = []
        for (k, n), (name, comment) in sorted(final.items()):
            c = f"   \\ ({k} {n}) {comment}" if comment else f"   \\ ({k} {n})"
            block.append((f"{n} constant {name}").ljust(36) + c)
        if heading in text:
            text = re.sub(re.escape(heading) + r"\n(?:[^\n]+\n)*", lambda m: m.group(0) + "\n".join(block) + "\n", text, count=1)
        else:
            block = ["", heading] + block
            text = re.sub(r"^(USING:[^\n]*\n)", lambda m: m.group(1) + "\n".join(block) + "\n", text, count=1, flags=re.M)
        f.write_text(text, encoding="latin-1")
    print(f"{sum(len(v) for v in rooms.values())} names in {len(rooms)} rooms")


if __name__ == "__main__":
    main(sys.argv[1:])
