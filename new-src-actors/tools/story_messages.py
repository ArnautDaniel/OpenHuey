#!/usr/bin/env python3
"""story_messages.py - each message the rooms show, its text beside it as a comment.

The scripts name messages by number (`6 message`: the room's own table; `$8012`: the system's,
bit 15; bit 14 the tutorials'). This runs the game headless (it needs the game's data): it goes
to each room in turn and lays out the messages that room's script names, then writes each one's
text after the line that shows it - `6 message   \\ "The gate is closed. ..."` - where the line has
no comment yet. A room with no data on the disc (off-map-5a) gets none; the shared
scripts get only the system's and the tutorials' (they run in any room).

    tools/story_messages.py [path/to/hga]     (default: ../build/new-src-actors/hga)
"""
import os
import pathlib
import re
import subprocess
import sys
import tempfile

ROOT = pathlib.Path(__file__).resolve().parent.parent
ROOMS = sorted((ROOT / "scripts/story/rooms").glob("*.fs"))
SHARED = ROOT / "scripts/story/shared.fs"
SHOWS = r"(message|examine-message|message-prepare)"
USE = re.compile(r"(?<!\S)(\$[0-9A-Fa-f]+|\d+)(\s+)" + SHOWS + r"(?!\S)")
WIDTH = 110


def number(t):
    return int(t[1:], 16) if t.startswith("$") else int(t)


def code(text):
    return re.sub(r"\\[^\n]*", "", text)


def global_id(v):
    return v & 0xC000 != 0


def wanted():
    rooms, glob_ids = {}, set()
    for f in [*ROOMS, SHARED]:
        for m in USE.finditer(code(f.read_text())):
            v = number(m.group(1))
            if v == 0xFFFF:
                continue
            if global_id(v) or f == SHARED:
                glob_ids.add(v)
            else:
                rooms.setdefault(f.stem, set()).add(v)
    return rooms, glob_ids


def probe(rooms, glob_ids):
    lines = ["IN: msgdump", "USING: engine game-state actors messages room-names game ;",
             ": frames  0 ?do game-tick loop ;",
             ': .text ( id -- )  dup . ." |" message-layout 0 ?do  i message-lines 0 ?do  j i message-line type ."  "  loop  loop  cr ;',
             ": .room ( room -- )  . room-id . cr ;",
             # (a frame at a time until it is in: a room may send her straight on - water-tower-7f-1)
             ": arrive ( room -- )  20 0 do  1 frames  room-id over = if  leave  then  loop  drop ;"]
    for r, ids in sorted(rooms.items()):
        lines.append(f'{r} -1 rooms send go-to-room  {r} arrive  ." @{r} " {r} .room')
        lines.append(" ".join(f"{i} .text" for i in sorted(ids)))
    lines.append('." @* -1 -1 " cr')
    lines.append(" ".join(f"{i} .text" for i in sorted(glob_ids)))
    return "\n".join(lines) + "\n"


def run(hga, text):
    with tempfile.NamedTemporaryFile("w", suffix=".fs", delete=False) as t:
        t.write(text)
    env = dict(os.environ, SDL_AUDIO_DRIVER="dummy")
    out = subprocess.run([hga, "--test", t.name], cwd=ROOT, env=env, capture_output=True, timeout=1200).stdout
    os.unlink(t.name)
    return out.decode("latin-1")


def clean(text):
    text = re.sub(r"[^\x20-\x7E]", "?", text)
    return re.sub(r"\s+", " ", text).strip().replace('"', "'")


def parse(out):
    """{room name or '*': {id: text}} for the rooms that came in as asked"""
    texts, cur = {}, None
    for line in out.splitlines():
        if line.startswith("@"):
            name, want, got = (line[1:].split() + ["", ""])[:3]
            cur = {} if want == got else None
            if cur is not None:
                texts[name] = cur
            continue
        m = re.match(r"(\d+) \|(.*)", line)
        if m and cur is not None:
            cur[int(m.group(1))] = clean(m.group(2))
    return texts


def annotate(f, local, glob_texts):
    out, n = [], 0
    for line in f.read_text().split("\n"):
        m = USE.search(code(line))
        if m and "\\" not in line:
            v = number(m.group(1))
            text = glob_texts.get(v) if global_id(v) else local.get(v)
            if text:
                room = WIDTH - len(line) - 7
                if room > 12:
                    if len(text) > room:
                        text = text[: room - 3].rstrip() + "..."
                    line = f'{line}   \\ "{text}"'
                    n += 1
        out.append(line)
    f.write_text("\n".join(out))
    return n


def main(args):
    hga = args[0] if args else str(ROOT.parent / "build/new-src-actors/hga")
    rooms, glob_ids = wanted()
    out = run(hga, probe(rooms, glob_ids))
    texts = parse(out)   # (each room: its name, its number as asked and as it came in)
    glob_texts = texts.get("*", {})
    total = 0
    for f in ROOMS:
        total += annotate(f, texts.get(f.stem, {}), glob_texts)
    total += annotate(SHARED, {}, glob_texts)
    missing = sorted(set(rooms) - set(texts))
    print(f"{total} messages written beside; rooms not come in: {' '.join(missing) or 'none'}")


if __name__ == "__main__":
    main(sys.argv[1:])
