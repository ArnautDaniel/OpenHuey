#!/usr/bin/env python3
"""story_idioms.py - the converted scripts' repeated idioms as the story words that say them
(run once over the converted rooms). Each idiom is the exact sequence of words the converter
wrote; a copy that differs at all is left as it is. Comments between its words stop a match too.

    tools/story_idioms.py      (scripts/story/rooms/*.fs and scripts/story/shared.fs)
"""
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
FILES = [*sorted((ROOT / "scripts/story/rooms").glob("*.fs")), ROOT / "scripts/story/shared.fs"]

IDIOMS = [
    ("scene-wait-ready", "2 cutscene-control begin 3 cutscene-control 2 cutscene-mode? not while yield repeat"),
    ("scene-play", "6 cutscene-control 1.0 movie-volume effects-arena-flip yield movie-skipped state-flag-clear "
                   "$F 1 fade $A cutscene-control begin movie-skipped state-flag? not if 7 cutscene-control "
                   "cutscene-near-end? if $F 0 fade then $C cutscene-control 0 cutscene-control 4 cutscene-control "
                   "-1 result? not if 5 cutscene-mode? not 4 cutscene-mode? not and if yield false else true then "
                   "else true then else true then until $FFFF message-close $F9 action-end $B cutscene-control "
                   "wait-fade 1 cutscene-control world-held state-flag-set effects-arena-flip begin movie-playing? "
                   "while yield repeat yield pause-wanted state-flag-set 8 cutscene-control"),
    ("item-fanfare", "0 $F9 $8C action-force $83 $85 0.0 0.0 0.0 0 0 sound"),
    ("panic-eased", "$FF panic-stage? if 3 panic-stage then"),
    ("in-panic?", "$FF panic-stage?"),
    ("scene-play-through", "6 cutscene-control 1.0 movie-volume effects-arena-flip yield $F 1 fade $A cutscene-control "
                           "begin 7 cutscene-control $C cutscene-control 0 cutscene-control 4 cutscene-control "
                           "-1 result? not if 5 cutscene-mode? not 4 cutscene-mode? not and if yield false else true "
                           "then else true then until"),
]


# with places (`S`, `P`: a literal number each, the same wherever the name is used again)
SHAPED = [
    ("S P player-camera", "hewie-controlled? not if fiona-who S P char-camera fiona-who camera-follow "
                          "else hewie-who S P char-camera hewie-who camera-follow then"),
]


def pattern(seq):
    return re.compile(r"(?<!\S)" + r"\s+".join(re.escape(t) for t in seq.split()) + r"(?!\S)")


NUMBER = r"(-?\$?[0-9A-Fa-f]+)"


def shaped(seq):
    """a pattern whose single capital letters are numbers (named groups; again: the same)"""
    parts, seen = [], set()
    for t in seq.split():
        if re.fullmatch(r"[A-Z]", t):
            parts.append(f"(?P={t})" if t in seen else f"(?P<{t}>{NUMBER[1:-1]})")
            seen.add(t)
        else:
            parts.append(re.escape(t))
    return re.compile(r"(?<!\S)" + r"\s+".join(parts) + r"(?!\S)")


# Fiona's action offered: `5 act kind scene-change` (scene 5 always) - its kind by when it may be
WHEN = {0: "unhurried", 1: "always", 7: "always", 2: "calm", 4: "calm", 5: "calm", 6: "calm", 3: "calm-hewie-up"}
OFFER = re.compile(r"(?<!\S)5\s+(\S+)\s+([0-7])\s+scene-change(?!\S)")


def offers(text):
    return OFFER.subn(lambda m: f"{m.group(1)} {WHEN[int(m.group(2))]} offer-action", text)


# the nav flags by name (scripts/floor-flags.fs), a mask as its flags or-ed; set / cleared on a
# group (nav-group) or a triangle (nav-tri-flags): floor-on / floor-off, tri-floor-on / -off;
# the rooms' model parts (door-bits on base $14): part-shown / part-hidden
def floor_names():
    out = {}
    for line in (ROOT / "scripts/floor-flags.fs").read_text().split("\n"):
        for t, name in re.findall(r"(?<!\S)(\$?[0-9A-Fa-f]+) constant (\S+)", line.split("\\")[0]):
            out.setdefault(int(t[1:], 16) if t.startswith("$") else int(t), name)
    return out


FLOORS = floor_names()
SINGLE = {v: n for v, n in FLOORS.items() if v & (v - 1) == 0}


def mask(t):
    v = int(t[1:], 16) if t.startswith("$") else int(t)
    if v in FLOORS:
        return FLOORS[v]
    names, rest = [], v
    if rest & 0x2008000 == 0x2008000:
        names.append("water-floor")
        rest &= ~0x2008000
    for b in range(32):
        if rest & (1 << b):
            if (1 << b) not in SINGLE:
                return None
            names.append(SINGLE[1 << b])
    return names[0] + "".join(f" {n} or" for n in names[1:])


NAV = re.compile(r"(?<!\S)([01])\s+(\S+)\s+(\$?[0-9A-Fa-f]+)\s+(nav-group|nav-tri-flags)(?!\S)")
PARTS = re.compile(r"(?<!\S)(\S+)\s+([01])\s+\$14\s+door-bits(?!\S)")


def floors(text):
    def nav(m):
        names = mask(m.group(3))
        if names is None:
            return m.group(0)
        word = ("floor-" if m.group(4) == "nav-group" else "tri-floor-") + ("on" if m.group(1) == "1" else "off")
        return f"{m.group(2)} {names} {word}"
    text, a = NAV.subn(nav, text)
    text, b = PARTS.subn(lambda m: f"{m.group(1)} part-{'shown' if m.group(2) == '1' else 'hidden'}", text)
    return text, a + b


def main():
    pats = [(name, pattern(seq)) for name, seq in IDIOMS]
    pats += [(name, shaped(seq)) for name, seq in SHAPED]
    counts = dict.fromkeys((name for name, _ in pats), 0)
    for f in FILES:
        s = t = f.read_text()
        for name, p in pats:
            repl = (lambda m, name=name: " ".join(m.group(w) if re.fullmatch(r"[A-Z]", w) else w
                                                  for w in name.split()))
            t, n = p.subn(repl, t)
            counts[name] += n
        t, n = offers(t)
        counts["offer-action"] = counts.get("offer-action", 0) + n
        t2, n = floors(t)
        if n and "floor-flags" not in t2.split("\n", 8)[4] + t2.split("\n", 8)[5]:
            t2 = re.sub(r"^(USING:[^;\n]*?)( ;)", r"\1 floor-flags\2", t2, count=1, flags=re.M)
        t = t2
        counts["floors, parts"] = counts.get("floors, parts", 0) + n
        if t != s:
            f.write_text(t)
    for name, n in counts.items():
        print(f"{name}: {n}")


if __name__ == "__main__":
    main()
