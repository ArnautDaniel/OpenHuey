#!/usr/bin/env python3
"""story_who.py - the rooms' character arguments by name (run once over the converted scripts).

The converted scripts pass characters by their script id: 0 Fiona, 1 Hewie, $FE the stalker, $FF
the script's own (`0 5 char-in-area?`). Before each word that takes a character, a literal in
that argument's place becomes `fiona-who`, `hewie-who`, `stalker-who` or `self-who`. Only
straight-line literals are touched: walking back from the word, float literals are skipped (they
are on the other stack) and anything else ends the look.

    tools/story_who.py            (scripts/story/rooms/*.fs and scripts/story/shared.fs)
"""
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
FILES = [*sorted((ROOT / "scripts/story/rooms").glob("*.fs")), ROOT / "scripts/story/shared.fs"]

# word: its character arguments' places, counted from the top of the data stack (0: the last)
WHO = {
    "action": [1], "action-force": [1], "action-end": [0], "self-is?": [0], "char-here?": [0],
    "char-busy?": [0], "char-in-area?": [1], "char-entered-area?": [1], "char-left-area?": [1],
    "char-on-tri?": [1], "char-in-room?": [1], "char-heading?": [2], "char-to-exit": [1],
    "char-camera": [2], "camera-follow": [0], "char-sound": [2], "char-to-tri": [1],
    "char-to-tri-facing": [2], "char-to-xz": [2], "char-to-xyz": [1], "char-animal-go": [1],
    "char-shift": [0], "char-faces-xz?": [3], "char-faces-area?": [2], "char-action?": [1],
    "char-zone-bits?": [2], "char-zone-bits-before?": [2], "char-in-zone?": [1],
    "self-turn-to": [0], "self-look-at": [0], "self-move-slot": [1], "char-wait-done": [0],
    "wait-char-anim": [0], "char-anim-hold": [3], "char-visible": [1],
    "char-at-motion-event?": [0], "speak": [1], "speech-off": [0], "char-effect-moving": [1],
    "char-done": [0], "char-cond?": [1], "char-dead?": [0], "char-full-health?": [0],
    "char-heading-for?": [1], "char-unseen?": [0], "char-cond!": [1], "char-full-health": [0],
    "char-heal": [1], "char-silent": [1], "char-activate": [0], "stalker-command": [1],
    "stalker-search-delay": [1], "stalker-knock-down": [0], "stalker-rage": [1],
    "char-wait-motion": [1], "hewie-look-char": [0], "self-touching?": [0],
    "char-touching-facing?": [4, 3], "chars-within?": [2, 1], "char-tri": [0],
    "char-in-nav-group?": [1], "char-on-nav-flags?": [1], "char-before-door?": [1],
    "char-not-at-door?": [1], "char-tri-free?": [0], "char-no-shadow": [1], "char-file-load": [1],
    "char-file-use": [0], "door-lock-for": [2], "room-doors-state": [2], "splash": [6],
    "fiona-target": [1], "fiona-thrown": [1], "char-find-tri": [0], "char-tint": [2],
    "char-layer": [1], "show-part": [2], "hide-part": [1], "daniella?": [0], "char-model-op": [1],
    "doorway-fade!": [1],
}
NAMES = {0: "fiona-who", 1: "hewie-who", 0xFE: "stalker-who", 0xFF: "self-who"}

TOKEN = re.compile(r"\S+")
INT = re.compile(r"-?(\$[0-9A-Fa-f]+|\d+)$")
FLOAT = re.compile(r"-?(\d+\.\d*(e-?\d+)?|\d+e-?\d*|\.\d+)$")


def tokens(text):
    """The code's tokens (start, end, text), comments and strings left out."""
    out, i, n = [], 0, len(text)
    while True:
        m = TOKEN.search(text, i)
        if not m:
            return out
        t = m.group()
        if t == "\\":
            nl = text.find("\n", m.end())
            i = n if nl < 0 else nl
        elif t == "(":
            i = text.find(")", m.end()) + 1 or n
        elif t in ('s"', '."', 'abort"'):
            i = text.find('"', m.end()) + 1 or n
        else:
            out.append((m.start(), m.end(), t))
            i = m.end()


def value(t):
    t = t.lstrip("-")
    return int(t[1:], 16) if t.startswith("$") else int(t)


def convert(text):
    toks = tokens(text)
    edits = {}
    for k, (_, _, t) in enumerate(toks):
        places = WHO.get(t)
        if not places:
            continue
        want, n, j = max(places), 0, k - 1
        while j >= 0 and n <= want:
            p = toks[j][2]
            if FLOAT.match(p):
                j -= 1
                continue
            if not INT.match(p):
                break
            if n in places and not p.startswith("-") and value(p) in NAMES:
                edits[toks[j][0]] = (toks[j][1], NAMES[value(p)])
            n += 1
            j -= 1
    out, last = [], 0
    for a in sorted(edits):
        b, name = edits[a]
        out.append(text[last:a])
        out.append(name)
        last = b
    out.append(text[last:])
    return "".join(out), len(edits)


def main():
    total = 0
    for f in FILES:
        s = f.read_text()
        t, n = convert(s)
        if n:
            f.write_text(t)
            total += n
    print(f"{total} character arguments named")


if __name__ == "__main__":
    main()
