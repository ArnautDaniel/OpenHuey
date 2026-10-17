#!/usr/bin/env python3
"""story_flow.py - the converted scripts' waits, messages and animations as the words that say
them, and their Fiona-or-Hewie branches as one (run once over the converted rooms).

- `self-frames-reset N self-wait-frames` -> `N wait-frames` (self-wait-16: 16)
- `A self-anim self-wait-anim` -> `A play-out`; `A B self-anim-blend self-wait-anim` -> `A B blend-out`
- `N message wait-message` -> `N say`
- in an action script: `begin X not while yield repeat` -> `[: X ;] wait-until`, `begin X while
  yield repeat` -> `[: X ;] wait-while` (X a few words, no control flow; a phase script can't
  wait, so its loops are left alone)
- `hewie-controlled? not if A else B then`, B being A with Hewie where A has Fiona -> A with
  `player-who` (the one played)

    tools/story_flow.py      (scripts/story/rooms/*.fs, scripts/story/shared.fs)
"""
import collections
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
FILES = [*sorted((ROOT / "scripts/story/rooms").glob("*.fs")), ROOT / "scripts/story/shared.fs"]
COUNT = collections.Counter()


def seq(words):
    return r"(?<!\S)" + r"[ \t]*\n?[ \t]*".join(words) + r"(?!\S)"


TOK = r"(\S+)"
SIMPLE = [
    ("wait-frames", re.compile(seq([r"self-frames-reset", TOK, r"self-wait-frames"])), lambda m: f"{m.group(1)} wait-frames"),
    ("wait-frames", re.compile(seq([r"self-frames-reset", r"self-wait-16"])), lambda m: "16 wait-frames"),
    ("play-out", re.compile(seq([TOK, r"self-anim", r"self-wait-anim"])), lambda m: f"{m.group(1)} play-out"),
    ("blend-out", re.compile(seq([TOK, TOK, r"self-anim-blend", r"self-wait-anim"])), lambda m: f"{m.group(1)} {m.group(2)} blend-out"),
    ("say", re.compile(seq([TOK, r"message", r"wait-message"])), lambda m: f"{m.group(1)} say"),
    ("say", re.compile(r'(?<!\S)(\S+)[ \t]+message([ \t]+\\ "[^\n]*")?\n[ \t]*wait-message(?!\S)'),
     lambda m: f"{m.group(1)} say{m.group(2) or ''}"),   # (its text beside it kept)
]
FLOW = {"if", "else", "then", "begin", "while", "repeat", "until", "again", "do", "?do", "loop", "+loop",
        "case", "of", "endof", "endcase", "exit", ";", ":", "[:", ";]", "\\", "(", 's"', ".\""}
LOOP = re.compile(r"(?<!\S)begin\s+((?:\S+\s+){1,8}?)(not\s+)?while\s+yield\s+repeat(?!\S)")


def loops(body):
    def one(m):
        x = m.group(1).split()
        if any(t in FLOW for t in x):
            return m.group(0)
        word = "wait-until" if m.group(2) else "wait-while"
        COUNT[word] += 1
        return f"[: {' '.join(x)} ;] {word}"
    return LOOP.sub(one, body)


def player(text):
    """hewie-controlled? not if A else B then -> A with player-who (B: A with Hewie for Fiona)"""
    out, i = [], 0
    for m in re.finditer(r"(?<!\S)hewie-controlled\?\s+not\s+if(?!\S)", text):
        if m.start() < i:
            continue
        # the matching else and then (at depth 0)
        depth, pos, else_at, then_at = 0, m.end(), None, None
        for t in re.finditer(r"\S+", text[m.end():]):
            w = t.group()
            if w in ("if",):
                depth += 1
            elif w == "then":
                if depth == 0:
                    then_at = (m.end() + t.start(), m.end() + t.end())
                    break
                depth -= 1
            elif w == "else" and depth == 0 and else_at is None:
                else_at = (m.end() + t.start(), m.end() + t.end())
            elif w in ("\\", "(", 's"'):
                break
        if not else_at or not then_at:
            continue
        a = text[m.end():else_at[0]]
        b = text[else_at[1]:then_at[0]]
        if "fiona-who" not in a or a.split() and b.split() != a.replace("fiona-who", "hewie-who").split():
            continue
        line_start = text.rfind("\n", 0, m.start()) + 1
        indent = text[line_start:m.start()]
        body = a.replace("fiona-who", "player-who").strip("\n")
        lines = [l[4:] if l.startswith(indent + "    ") else l for l in body.split("\n")]
        new = "\n".join(lines).strip()
        out.append(text[i:m.start()] + new)
        i = then_at[1]
        COUNT["player-who"] += 1
    out.append(text[i:])
    return "".join(out)


def convert(text):
    for name, pat, repl in SIMPLE:
        text, n = pat.subn(repl, text)
        COUNT[name] += n
    # the loops: in action scripts only
    text = re.sub(r"^: \S+\.act[0-9A-F]{2} .*?^;", lambda m: loops(m.group(0)), text, flags=re.M | re.S)
    return player(text)


def main():
    for f in FILES:
        s = f.read_text(encoding="latin-1")
        t = convert(s)
        if t != s:
            f.write_text(t, encoding="latin-1")
    for k, v in COUNT.items():
        print(f"{k}: {v}")


if __name__ == "__main__":
    main()
