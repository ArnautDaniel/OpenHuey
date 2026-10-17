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
        if t != s:
            f.write_text(t)
    for name, n in counts.items():
        print(f"{name}: {n}")


if __name__ == "__main__":
    main()
