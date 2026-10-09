#!/usr/bin/env python3
"""The rooms' event scripts for new-src-actors, from new-src's conversion of the game's bytecode
(new-src/scripts/events/map*.fs, other.fs, builtin.fs - new-src is archived at the git tag
new-src-final: check it out to run this again): one file a room, the rooms by name.

  scripts/story/rooms/<name>.fs   IN: story.rooms.<name>   its words <name>.enter, .phase1 ...,
                                  .act05 ..., and their registration (room-script! /
                                  action-script!) by the room's name
  scripts/story/shared.fs         IN: story.shared   the shared scripts (shared.act80 ...)
  scripts/story/rooms.fs          IN: story.rooms    loads them all

Room numbers in the words' names and the registrations become the names in
scripts/room-names.fs. Run once: the files are the source afterwards (edit them by hand).

usage: story_convert.py   (from new-src-actors/)"""
import glob
import os
import re

here = os.path.dirname(os.path.abspath(__file__))
top = os.path.join(here, '..')
src = os.path.join(top, '..', 'new-src', 'scripts', 'events')

names = {}
for line in open(os.path.join(top, 'scripts', 'room-names.fs')):
    m = re.match(r'\$([0-9A-F]+)\s+constant\s+(\S+)', line)
    if m:
        names[int(m.group(1), 16)] = m.group(2)


def name(r):
    return names[r]


ROOM = re.compile(r'\broom([0-9A-F]{2,3})\.')


def rename(text):
    text = ROOM.sub(lambda m: name(int(m.group(1), 16)) + '.', text)
    return text.replace('builtin.', 'shared.')


def registration(line):
    """' roomXX.word $XX n room-script!  ->  ' name.word name n room-script!"""
    m = re.match(r"'\s+room([0-9A-F]+)\.(\S+)\s+\$([0-9A-F]+)\s+(\S+)\s+(room-script!|action-script!)\s*$", line)
    if not m:
        return None
    r = int(m.group(1), 16)
    return r, "' %s.%s %s %s %s" % (name(r), m.group(2), name(r), m.group(4), m.group(5))


rooms = {}      # room -> list of lines (its words)
regs = {}       # room -> registration lines
heads = {}      # room -> its section's header comment lines
for f in sorted(glob.glob(os.path.join(src, 'map*.fs'))) + [os.path.join(src, 'other.fs')]:
    cur = None
    for line in open(f).read().split('\n'):
        m = re.match(r'\\ ---- room \$([0-9A-F]+) -', line)
        if m:
            cur = int(m.group(1), 16)
            rooms.setdefault(cur, [])
            continue
        if line.startswith('IN:') or line.startswith('USING:') or (cur is None and line.startswith('\\')):
            continue
        reg = registration(line)
        if reg:
            regs.setdefault(reg[0], []).append(reg[1])
            continue
        if cur is not None:
            rooms[cur].append(line)

os.makedirs(os.path.join(top, 'scripts', 'story', 'rooms'), exist_ok=True)


def area_of(r):
    for line in open(os.path.join(top, 'scripts', 'room-names.fs')):
        m = re.match(r'\$([0-9A-F]+)\s+constant\s+\S+\s+\\ (.*)', line)
        if m and int(m.group(1), 16) == r:
            return m.group(2)
    return ''


for r, lines in sorted(rooms.items()):
    body = '\n'.join(lines).strip('\n')
    uses = sorted({int(x, 16) for x in ROOM.findall(body)} - {r})
    using = ['room-names', 'story.words', 'story.shared'] + ['story.rooms.' + name(u) for u in uses]
    out = ['\\ story/rooms/%s.fs - the event scripts of room %s ($%X; %s).' % (name(r), name(r), r, area_of(r)),
           '\\ Converted from the game\'s bytecode (new-src\'s tools/events2forth.py) and renamed by',
           '\\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.',
           'IN: story.rooms.' + name(r),
           'USING: ' + ' '.join(using) + ' ;',
           '',
           rename(body),
           '']
    if regs.get(r):
        out += ['\\ ---- registered ----'] + regs[r] + ['']
    open(os.path.join(top, 'scripts', 'story', 'rooms', name(r) + '.fs'), 'w').write('\n'.join(out))

# the shared scripts
text = open(os.path.join(src, 'builtin.fs')).read()
text = text.replace('IN: events.builtin', 'IN: story.shared').replace('USING: events.core events.words ;', 'USING: story.words ;')
text = re.sub(r'^\\ events/builtin\.fs - .*\n(\\ Converted.*\n)?',
              '\\\\ story/shared.fs - the shared event scripts (ids 0x80..) and the built-in ones.\n'
              '\\\\ Converted from the game\'s bytecode (new-src\'s tools/events2forth.py), renamed by\n'
              '\\\\ tools/story_convert.py, once: edit by hand.\n', text)
open(os.path.join(top, 'scripts', 'story', 'shared.fs'), 'w').write(rename(text))

# everything loaded
order = [name(r) for r in sorted(rooms)]
lines = ['\\ story/rooms.fs - every room\'s event scripts (story/rooms/*.fs), loaded.',
         'IN: story.rooms']
for i in range(0, len(order), 6):
    lines.append(('USING: ' if i == 0 else '    ') + ' '.join('story.rooms.' + n for n in order[i:i + 6]))
lines[-1] += ' ;'
open(os.path.join(top, 'scripts', 'story', 'rooms.fs'), 'w').write('\n'.join(lines) + '\n')
print(len(rooms), 'rooms')
