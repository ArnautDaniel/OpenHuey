#!/usr/bin/env python3
"""Convert the rooms' event scripts (the game's bytecode) to Forth, once.

    events2forth.py              write scripts/events/*.fs (and events/words.fs, the word stubs)
    events2forth.py --stats      what the conversion did, without writing
    events2forth.py --room 2A    one room's Forth, to stdout

The bytecode is read with the decomp's tools (tools/evdis.py reads the scripts from the game's
executable, tools/event_opcodes.py names every command and condition and gives its operands);
see docs/event_opcodes.md for the bytecode. After the conversion the Forth files are the source:
they are edited by hand, and this tool stays only to check against the original.

How the bytecode maps onto Forth:
 - A script is a word: `room2A.enter`, `room2A.phase1`..`phase5`, `room2A.char-enter`,
   `room2A.act05` (action script 5); the shared ones (ids 0x80..) are `builtin.act80`.
 - A command is a word named for what it does, its operands before it in their order; a
   condition is a word leaving a flag (its name ends in `?`). A coordinate (1/1000 units in the
   bytecode) is written as a float, which goes on the float stack.
 - F0 / F1 + conditions joined by F2..F5 (left to right) become `c1 c2 and c3 not or if`.
 - F9 ("end, and skip the rest of the enclosing block") becomes an `else`: what followed in the
   enclosing block moves into the branch that didn't take the F9.
 - The loop point (0x23) and loop back (0x24) become `begin ... while ... repeat` (or
   `begin ... until`); a loop of another shape keeps a flag: `begin ... false (again) ...
   true (out) until`.
 - Going to another script (0x25) is `['] target goto` (the task goes on in it and never comes
   back); calling one (0x2C) is the word itself; returning (0x2D) is `exit`.
 - The room's own commands and conditions (0x22 / 0x11) are words of the room: `room2A.cmd00`,
   `room2A.cond00?`, with the bytes they're given; they start as stubs here, with the C
   function's description (src/game/rooms/room_XX.c), to be written in Forth.
 - Files follow the game's own areas: the maps Fiona finds (kMapRooms); a room in several goes
   with the first.
"""
import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / 'tools'))
import evdis           # noqa: E402
import event_opcodes as OPS   # noqa: E402

OUT = REPO / 'new-src/scripts/events'
CMD_LEN = evdis.cmd_lengths()
STATS = Counter()

# ---- reading the scripts ------------------------------------------------------------------------

PHASES = {'EnterScript': 'enter', 'Phase1Script': 'phase1', 'Phase2Script': 'phase2',
          'Phase3Script': 'phase3', 'Phase4Script': 'phase4', 'Phase5Script': 'phase5',
          'CharEnterScript': 'char-enter'}
PHASE_SLOT = {'enter': 0, 'phase1': 1, 'phase2': 2, 'phase3': 3, 'phase4': 4, 'phase5': 5,
              'char-enter': 6}
BUILTIN_TABLE = 'kBuiltinScripts'
BUILTIN_SPECIAL = {0x3D6240: 'after-phase1', 0x3D6230: 'after-phase2',   # (D_003D6240, ...)
                   0x3D6C10: 'char-enter-26'}
KMAPROOMS = 0x420B20


def u32(r, a):
    return int.from_bytes(r.bytes(a, 4), 'little')


def collect(r, syms):
    """[(owner, name, address)]: owner is a room number or 'builtin'"""
    out = []
    for room, name, addr in evdis.scripts(syms):
        if name.startswith('Action'):
            out.append((int(room, 16), 'act%02X' % int(name[6:]), addr))
        else:
            out.append((int(room, 16), PHASES[name], addr))
    table = syms[BUILTIN_TABLE]
    after = min(a for a in syms.values() if a > table)
    for i in range(0x80, min(0x100, (after - table) // 4)):
        a = u32(r, table + i * 4)
        if 0x3E0000 > a >= 0x3D0000 or 0x3E0000 <= a < 0x480000:
            out.append(('builtin', 'act%02X' % i, a))
    for addr, name in BUILTIN_SPECIAL.items():
        out.append(('builtin', name, addr))
    return out


def areas(r):
    """room -> map number (the first map it is on)"""
    out = {}
    i = 0
    while True:
        m = u32(r, KMAPROOMS + 4 * i)
        if m == 0:
            return out
        a = m
        while u32(r, a) != 0xFFFFFFFF:
            out.setdefault(u32(r, a), i)
            a += 0x18
        i += 1


# ---- the script as a tree ---------------------------------------------------------------------

class Cmd:
    def __init__(self, e):
        self.addr, self.op, self.raw = e[0], e[2], bytes(e[3])


class If:
    """F0 / F1 + chain (or FA / F6: no chain), then-part, else-part (None: none), end op"""
    def __init__(self, opener, chain):
        self.opener, self.chain = opener, chain
        self.then, self.els, self.end = [], None, 0xF8


class Mark:
    pass


class Dead:
    """code the original can never run (after a stray else at the top level)"""
    def __init__(self, items):
        self.items = items


def build(ev):
    top = []
    stack = []      # (If or None, the list it sits in)
    cur = top
    i = 0
    while i < len(ev):
        a, kind, op, raw, _ = ev[i]
        if kind == 'ctl' and op in (0xF0, 0xF1):
            chain = [(op, bytes(ev[i + 1][3]))]
            i += 2
            while i < len(ev) and ev[i][1] == 'ctl' and 0xF2 <= ev[i][2] <= 0xF5:
                chain.append((ev[i][2], bytes(ev[i + 1][3])))
                i += 2
            node = If(op, chain)
            cur.append(node)
            stack.append((node, cur))
            cur = node.then
            continue
        i += 1
        if kind == 'ctl' and op in (0xF6, 0xFA):
            node = If(op, [])
            cur.append(node)
            stack.append((node, cur))
            cur = node.then
        elif kind == 'ctl' and op == 0xF7:
            if not stack:   # (an else with no block open: the original skips to the next end)
                STATS['stray else'] += 1
                d = Dead([])
                cur.append(d)
                stack.append((None, cur))
                cur = d.items
            else:
                stack[-1][0].els = []
                cur = stack[-1][0].els
        elif kind == 'ctl' and op in (0xF8, 0xF9, 0xFB):
            if not stack:
                STATS['stray end'] += 1
                continue
            node, cur = stack.pop()
            if node is not None:
                node.end = op
        elif kind == 'cmd':
            cur.append(Mark() if op == 0x23 else Cmd(ev[i - 1]))
    if stack:
        raise ValueError('%d blocks left open' % len(stack))
    return top


# ---- where control goes -----------------------------------------------------------------------
# FALL: on past the end; SKIP: out of the enclosing block (F9); CONT: back to the loop point;
# END: out of the script (go to, return, end of the action)

ENDS = (0x25, 0x2D, 0x06)


def exits(seq):
    s = {'FALL'}
    for k, x in enumerate(seq):
        if 'FALL' not in s:
            break
        s.discard('FALL')
        if isinstance(x, Cmd):
            s.add('CONT' if x.op == 0x24 else 'END' if x.op in ENDS else 'FALL')
        elif isinstance(x, Mark):
            s |= exits(seq[k + 1:]) - {'CONT'}
            return s
        elif isinstance(x, Dead):
            s.add('FALL')
        else:
            s |= if_exits(x)
    return s


def branch_exits(x):
    """(then, else) exits as they leave the if (a SKIP from inside a branch only leaves it)"""
    t = {'FALL' if e == 'SKIP' else e for e in exits(x.then)}
    e = {'FALL' if e == 'SKIP' else e for e in exits(x.els)} if x.els is not None else {'FALL'}
    if x.opener in (0xF6, 0xFA):
        e = set()
    if x.end == 0xF9:   # the branch that runs into the F9 leaves the enclosing block too
        if x.els is None and 'FALL' in t:
            t = (t - {'FALL'}) | {'SKIP'}
        elif x.els is not None and 'FALL' in e:
            e = (e - {'FALL'}) | {'SKIP'}
    return t, e


def if_exits(x):
    t, e = branch_exits(x)
    return t | e


def has_mark(seq):
    for x in seq:
        if isinstance(x, Mark):
            return True
        if isinstance(x, If) and (has_mark(x.then) or (x.els and has_mark(x.els))):
            return True
    return False


# ---- operands -----------------------------------------------------------------------------------

def num(v):
    return str(v) if 0 <= v < 10 else ('$%X' % v if v > 0 else str(v))


def fx(v):
    s = '%.3f' % (v / 1000.0)
    s = s.rstrip('0')
    return s + '0' if s.endswith('.') else s


def operands(spec, raw):
    """the operands as Forth literals, and their names for the stack comment (floats: F:)"""
    out, ints, floats = [], [], []
    at = 1
    for name, typ in OPS.parse(spec):
        if typ == 'str':   # (its bytes, then how many: the length byte before it isn't passed)
            n = raw[at - 1]
            out += [num(b) for b in raw[at:at + n]] + [str(n)]
            ints.append(name + '.. n')
            at += n
            continue
        if name == 'len' and spec.endswith(':str'):
            at += 1
            continue
        n = OPS.SIZES[typ]
        if at + n > len(raw):
            raise ValueError('operands past the end: %s %s' % (spec, raw.hex()))
        v = int.from_bytes(raw[at:at + n], 'big')
        at += n
        if typ in ('s8', 's16', 's32', 'fx', 'deg') and v >= 1 << (8 * n - 1):
            v -= 1 << (8 * n)
        if name == '_' or typ == 'pad':
            continue
        if typ == 'fx':
            out.append(fx(v))
            floats.append(name)
        else:
            out.append(str(v) if typ in ('s8', 's16', 's32', 'deg') else num(v))
            ints.append(name)
    for b in raw[at:]:   # (bytes the table doesn't name: passed on as they are)
        out.append(num(b))
        ints.append('b')
    return out, ints, floats


def entry(kind, raw):
    """(name, spec) of a command or condition, with the 0x59 / 0x50 sub-commands"""
    op = raw[0]
    if kind == 'cmd':
        if op == 0x59 and raw[1] in OPS.FLAGS:
            return OPS.FLAGS[raw[1]][:2]
        if op == 0x50 and raw[1] in OPS.PLACED:
            return OPS.PLACED[raw[1]][:2]
        return OPS.CMD[op][:2]
    return OPS.COND[op][:2]


# ---- Forth -------------------------------------------------------------------------------------

class Script:
    def __init__(self, owner, name, addr):
        self.owner, self.name, self.addr = owner, name, addr
        self.word = ('builtin.' if owner == 'builtin' else 'room%02X.' % owner) + name
        self.refs = set()       # words it names (calls, gotos)
        self.room_words = []    # (kind 'cmd' / 'cond', index, byte count)


class Conv:
    def __init__(self, scripts_by_owner, varying):
        self.by_owner = scripts_by_owner   # owner -> {name: Script}
        self.varying = varying             # room words given different byte counts: get the count

    def room_word(self, s, kind, raw):
        n = raw[2]
        args = [num(b) for b in raw[3:3 + n]]
        if (s.owner, kind, raw[1]) in self.varying:
            args.append(str(n))
        s.room_words.append((kind, raw[1], n))
        name = 'room%02X.%s%02X%s' % (s.owner, kind, raw[1], '?' if kind == 'cond' else '')
        return ' '.join(args + [name])

    # -- one command / condition
    def target(self, s, sid):
        """the word for script id `sid` seen from script s (None: only known at run time)"""
        if sid & 0x80:
            t = self.by_owner['builtin'].get('act%02X' % sid)
        elif s.owner == 'builtin':
            return None
        else:
            t = self.by_owner[s.owner].get('act%02X' % sid)
        if t is None:
            STATS['missing script target'] += 1
            return None
        return t

    def cmd(self, s, c):
        op, raw = c.op, c.raw
        if op == 0x24:
            raise AssertionError('loop back handled by the caller')
        if op == 0x2D:
            return 'exit'
        if op in (0x25, 0x2C):
            t = self.target(s, raw[1])
            if t is None:
                word = '%s %s' % (num(raw[1]), 'goto-action' if op == 0x25 else 'call-action')
                if s.owner == 'builtin':
                    return word
                return word + '   \\ (this room has no script %s: the original reads past its table)' % num(raw[1])
            s.refs.add(t.word)
            return "['] %s goto" % t.word if op == 0x25 else t.word
        if op == 0x22:
            n = raw[2]
            args = ' '.join(num(b) for b in raw[3:3 + n])
            if s.owner == 'builtin':
                return ('%s %d %s room-command' % (args, n, num(raw[1]))).strip()
            return self.room_word(s, 'cmd', raw)
        name, spec = entry('cmd', raw)
        if name.startswith('nop-'):
            STATS['no-effect commands dropped'] += 1
            return '\\ (%s: no effect in this game)' % name
        ops, _, _ = operands(spec, raw)
        return ' '.join(ops + [name])

    def cond(self, s, raw):
        if raw[0] == 0x11:
            n = raw[2]
            args = ' '.join(num(b) for b in raw[3:3 + n])
            if s.owner == 'builtin':
                return ('%s %d %s room-condition?' % (args, n, num(raw[1]))).strip()
            return self.room_word(s, 'cond', raw)
        name, spec = entry('cond', raw)
        ops, _, _ = operands(spec, raw)
        return ' '.join(ops + [name])

    def chain(self, s, x):
        parts = []
        for k, (op, raw) in enumerate(x.chain):
            c = self.cond(s, raw)
            if op in (0xF1, 0xF3, 0xF5):
                c += ' not'
            if k:
                c += ' and' if op in (0xF2, 0xF3) else ' or'
            parts.append(c)
        return ' '.join(parts)

    # -- sequences.  Lines are (depth, text).  `tail`: the lines to run when control reaches the
    # end of this block (by falling off it, or by an F9 just inside it); `loop`: the lines a loop
    # back becomes (None: no loop open).
    def seq(self, s, items, tail, loop, d):
        out = []
        for k, x in enumerate(items):
            rest = items[k + 1:]
            if isinstance(x, Mark):
                return out + self.loop_(s, rest, tail, loop, d)
            if isinstance(x, Dead):
                out.append((d, '\\ (never runs in the original: an else outside any block)'))
                for line in self.seq(s, x.items, [], loop, 0):
                    out.append((d, '\\   ' + line[1]))
                continue
            if isinstance(x, Cmd):
                if x.op == 0x24:
                    if loop is None:
                        raise ValueError('loop back with no loop point')
                    out += [(d + dd, t) for dd, t in loop]
                    STATS['dead after a jump'] += bool(rest)
                    return out
                out.append((d, self.cmd(s, x)))
                if x.op in ENDS:
                    STATS['dead after a jump'] += bool(rest)
                    return out
                continue
            t_ex, e_ex = branch_exits(x)
            if (t_ex | e_ex) <= {'FALL', 'END'}:
                out += self.if_(s, x, [], [], loop, d)
                continue
            # a branch leaves the block or loops: what follows goes into the branches that fall
            if has_mark(x.then) or (x.els and has_mark(x.els)):
                if 'CONT' in exits(rest):
                    STATS['loop point set in a branch, looped to after it'] += 1
            after = self.seq(s, rest, tail, loop, 0)
            t_tail = tail if 'SKIP' in t_ex else after
            e_tail = tail if 'SKIP' in e_ex else after
            if 'FALL' in t_ex and 'FALL' in e_ex and len(after) > 1:
                STATS['code repeated in both branches'] += 1
            return out + self.if_(s, x, t_tail, e_tail, loop, d)
        return out + [(d + dd, t) for dd, t in tail]

    def if_(self, s, x, t_tail, e_tail, loop, d):
        if x.opener in (0xF6, 0xFA):   # a plain block: always runs (an else in it never does)
            if x.els:
                STATS['else of an always-block dropped'] += 1
            return self.seq(s, x.then, t_tail, loop, d)
        out = [(d, self.chain(s, x) + ' if')]
        out += self.seq(s, x.then, t_tail, loop, d + 1)
        els = self.seq(s, x.els or [], e_tail, loop, d + 1)
        if els:
            # `a if X else b if Y else Z then then`: an else that is one if stays flat
            mid = [t for dd, t in els[1:-1] if dd == d + 1]
            if (len(els) >= 2 and els[0][0] == d + 1 and els[0][1].endswith(' if')
                    and els[-1][0] == d + 1 and els[-1][1].startswith('then')
                    and all(t == 'else' or t.startswith('else ') for t in mid)):
                out.append((d, 'else ' + els[0][1]))
                out += [(dd - 1, t) for dd, t in els[1:-1]]
                out.append((d, els[-1][1] + ' then'))
                return out
            out.append((d, 'else'))
            out += els
        out.append((d, 'then'))
        return out

    def loop_(self, s, region, tail, outer, d):
        STATS['loops'] += 1
        # begin <pre> <cond> while <body> repeat <post>: the first if whose branch always loops
        for k, x in enumerate(region):
            if isinstance(x, If) and x.chain and x.els is None:
                if exits(x.then) == {'CONT'} and not any(isinstance(y, (Mark, Dead)) for y in region[:k]) \
                        and all(not isinstance(y, If) or if_exits(y) <= {'FALL', 'END'} for y in region[:k]) \
                        and 'CONT' not in exits(region[k + 1:]) and not has_mark(x.then):
                    pre = self.seq(s, region[:k], [], None, d + 1)
                    body = x.then
                    cond = self.chain(s, x)
                    if len(body) == 1:   # (only the loop back: wait until the condition fails)
                        STATS['loops: begin until'] += 1
                        neg = cond[:-4] if cond.endswith(' not') and len(x.chain) == 1 else cond + ' not'
                        out = [(d, 'begin')] + pre + [(d + 1, neg + ' until')]
                    else:
                        STATS['loops: begin while repeat'] += 1
                        out = [(d, 'begin')] + pre + [(d + 1, cond + ' while')]
                        out += self.seq(s, body, [], [], d + 1)
                        out.append((d, 'repeat'))
                    return out + self.seq(s, region[k + 1:], tail, outer, d)
            if isinstance(x, (Mark, Dead)) or (isinstance(x, If) and not if_exits(x) <= {'FALL', 'END'}):
                break
            if isinstance(x, Cmd) and (x.op == 0x24 or x.op in ENDS):
                break
        if exits(region) == {'CONT'} or exits(region) == {'CONT', 'END'}:
            STATS['loops: begin again'] += 1
            return [(d, 'begin')] + self.seq(s, region, [], [], d + 1) + [(d, 'again')]
        # begin <body: true to leave, false to go round> until <what runs after it, once>
        STATS['loops: with a flag'] += 1
        k = next((k for k in range(len(region)) if 'CONT' not in exits(region[k:])
                  and 'SKIP' not in exits(region[:k])), len(region))
        out = [(d, 'begin')] + self.seq(s, region[:k], [(0, 'true')], [(0, 'false')], d + 1)
        return out + [(d, 'until')] + self.seq(s, region[k:], tail, outer, d)

    def word(self, s, tree):
        lines = self.seq(s, tree, [], None, 1)
        head = '%s ( -- )' % s.word
        out = [': ' + head + '   \\ %08X' % s.addr]
        out += ['    ' * dd + t for dd, t in lines]
        out.append(';')
        return out


# ---- the room's own commands and conditions -------------------------------------------------

def room_function_notes(room):
    """{('cmd' / 'cond', index): the comment above RoomXX_CmdNN / _CondNN}"""
    path = REPO / ('src/game/rooms/room_%02X.c' % room)
    notes = {}
    if not path.exists():
        return notes
    text = path.read_text()
    for m in re.finditer(r'((?:/\*(?:[^*]|\*(?!/))*\*/\s*)+)\w[\w *]*?\bRoom%02X_(Cmd|Cond)([0-9A-F]{2})\(' % room, text):
        comments = [c for c in re.findall(r'/\*((?:[^*]|\*(?!/))*)\*/', m.group(1))
                    if not re.match(r'\s*0x[0-9A-F]{8}\s*$', c)]
        body = ' '.join(' '.join(l.strip(' *') for l in c.split('\n')) for c in comments)
        notes[(m.group(2).lower(), int(m.group(3), 16))] = re.sub(r'\s+', ' ', body).strip()
    return notes


def wrap(text, width=96, lead='\\ '):
    out, line = [], ''
    for w in text.split():
        if line and len(line) + 1 + len(w) > width - len(lead):
            out.append(lead + line)
            line = w
        else:
            line = (line + ' ' + w).strip()
    if line:
        out.append(lead + line)
    return out


def room_stubs(room, uses):
    notes = room_function_notes(room)
    out = []
    for (kind, idx), counts in sorted(uses.items()):
        name = 'room%02X.%s%02X%s' % (room, kind, idx, '?' if kind == 'cond' else '')
        out += wrap(notes.get((kind, idx), 'Room%02X_%s%02X' % (room, kind.capitalize(), idx)))
        end = 'stub-flag' if kind == 'cond' else 'stub-step'
        if len(counts) > 1:   # (given different numbers of bytes: the count comes last)
            STATS['room words given different byte counts'] += 1
            args, body = 'bytes.. n', '0 ?do drop loop ' + end
        else:
            n = max(counts)
            args, body = ' '.join('b%d' % i for i in range(n)), ('drop ' * n + end)
        effect = '%s -- %s' % (args, 'flag' if kind == 'cond' else '')
        out.append(': %s ( %s )  %s ;' % (name, ' '.join(effect.split()), body))
    return out


# ---- the words file ------------------------------------------------------------------------------

CORE = {'yield'}


def stub(name, spec, desc, kind, tag, extra=0):
    _, ints, floats = operands_names(spec)
    ints += ['b'] * extra   # (bytes the table doesn't name)
    eff = ' '.join(ints)
    if floats:
        eff += ' F: ' + ' '.join(floats)
    eff = (eff + ' -- ' + ('flag' if kind == 'cond' else '')).strip()
    out = wrap('%s: %s' % (tag, desc))
    if name in CORE:
        return out + ['\\ (the core word `%s`)' % name]
    body = ' '.join(['0 ?do drop loop' if i.endswith('.. n') else 'drop' for i in reversed(ints)]
                    + ['fdrop'] * len(floats) + ['stub-flag' if kind == 'cond' else 'stub-step'])
    return out + [(': %s ( %s )  %s ;' % (name, eff, body)).replace('  ;', ' ;')]


def operands_names(spec):
    ints, floats = [], []
    for name, typ in OPS.parse(spec):
        if name == '_' or typ == 'pad' or (name == 'len' and spec.endswith(':str')):
            continue
        (floats if typ == 'fx' else ints).append(name + '.. n' if typ == 'str' else name)
    return None, ints, floats


def words_file():
    out = ['\\ events/words.fs - the words the converted event scripts are written in: one per',
           '\\ command and condition of the original bytecode (docs/event_opcodes.md). Generated',
           '\\ as stubs by tools/events2forth.py; each is to be written for real (in Forth on top of',
           '\\ C words), after which this file is the source.',
           'IN: events.words', '',
           '\\ what the stub conditions answer, and what the stub commands do (tests: random, yield)',
           'defer stub-flag  \' false is stub-flag',
           'defer stub-step  \' noop is stub-step',
           '',
           '\\ ---- running scripts', '',
           '\\ the task goes on in script `xt` and never comes back (the bytecode\'s 0x25)',
           ': goto ( xt -- )  drop stub-step ;',
           '\\ the same by id, in the current room (the shared scripts don\'t know their room)',
           ': goto-action ( id -- )  drop ;',
           ': call-action ( id -- )  drop ;',
           ': room-command ( bytes.. n cmd -- )  drop 0 ?do drop loop ;',
           ': room-condition? ( bytes.. n cond -- flag )  drop 0 ?do drop loop false ;',
           '\\ the scripts by room: slot 0 entering, 1..5 the phases, 6 a character entering; the',
           '\\ action scripts by room and id (0..$7F); the shared ones by id ($80..$FF). 0: none',
           '$110 constant rooms',
           'create room-scripts    rooms 7 * cells allot     room-scripts rooms 7 * cells 0 fill',
           'create action-scripts  rooms $80 * cells allot   action-scripts rooms $80 * cells 0 fill',
           'create builtin-scripts $80 cells allot           builtin-scripts $80 cells 0 fill',
           ': room-script ( room slot -- addr )  swap 7 * + cells room-scripts + ;',
           ': action-script ( room id -- addr )  swap $80 * + cells action-scripts + ;',
           ': builtin-script ( id -- addr )  $80 - cells builtin-scripts + ;',
           ': room-script! ( xt room slot -- )  room-script ! ;',
           ': action-script! ( xt room id -- )  action-script ! ;',
           ': builtin-script! ( xt id -- )  builtin-script ! ;',
           '']
    out += ['\\ ---- commands', '']
    for op in sorted(OPS.CMD):
        name, spec, desc = OPS.CMD[op]
        if op in (0x22, 0x23, 0x24, 0x25, 0x2C, 0x2D, 0x50, 0x59) or name.startswith('nop-'):
            continue
        size = OPS.spec_size(spec)
        extra = CMD_LEN[op] - 1 - size if size is not None and CMD_LEN[op] else 0
        out += stub(name, spec, desc, 'cmd', '%02X' % op, extra)
    for tag, table in (('59', OPS.FLAGS), ('50', OPS.PLACED)):
        for sub in sorted(table):
            name, spec, desc = table[sub]
            out += stub(name, spec, desc, 'cmd', '%s %02X' % (tag, sub))
    out += ['', '\\ ---- conditions', '']
    for op in sorted(OPS.COND):
        name, spec, desc = OPS.COND[op]
        if op == 0x11:
            continue
        out += stub(name, spec, desc, 'cond', '%02X' % op)
    return out


# ---- putting it together ---------------------------------------------------------------------

def order(scripts):
    """definitions before use; a script used before it can be defined is declared `defer`"""
    by_word = {s.word: s for s in scripts}
    done, out, deferred = set(), [], set()
    visiting = set()

    def visit(s):
        if s.word in done:
            return
        visiting.add(s.word)
        for w in sorted(s.refs):
            t = by_word.get(w)
            if t is None or t.word in done:
                continue
            if t.word in visiting:
                deferred.add(t.word)
                continue
            visit(t)
        visiting.discard(s.word)
        done.add(s.word)
        out.append(s)
    for s in scripts:
        visit(s)
    return out, deferred


def convert(only_room=None):
    r = evdis.Reader()
    evdis.READER = r
    syms = evdis.symbols()
    raw = collect(r, syms)
    evdis.STARTS.update(a for _, _, a in raw)
    evdis.STARTS.update(syms.values())
    room_area = areas(r)

    by_owner = defaultdict(dict)
    first_at = {}
    for owner, name, addr in raw:
        by_owner[owner][name] = Script(owner, name, addr)
    counts = defaultdict(set)
    for owner, name, addr in raw:
        if owner == 'builtin':
            continue
        for e in evdis.decode(r, addr):
            if (e[1], e[2]) in (('cmd', 0x22), ('cond', 0x11)):
                counts[(owner, e[1], e[3][1])].add(e[3][2])
    conv = Conv(by_owner, {k for k, v in counts.items() if len(v) > 1})

    texts = {}   # word -> lines
    alias = {}   # word -> the word of the same script converted first
    for owner, name, addr in raw:
        s = by_owner[owner][name]
        if addr in first_at:
            alias[s.word] = first_at[addr]
            STATS['scripts shared'] += 1
            continue
        first_at[addr] = s.word
        if only_room is not None and owner != only_room:
            continue
        tree = build(evdis.decode(r, addr))
        texts[s.word] = conv.word(s, tree)
        STATS['scripts'] += 1

    files = defaultdict(list)   # vocab -> lines
    uses_of = defaultdict(set)  # vocab -> vocabs it needs
    vocab_of = {}
    owners = sorted(by_owner, key=lambda o: (o == 'builtin', o if o != 'builtin' else 0))
    for owner in owners:
        vocab_of[owner] = 'events.builtin' if owner == 'builtin' else \
            'events.map%d' % room_area[owner] if owner in room_area else 'events.other'
    word_vocab = {}
    for owner in owners:
        for s in by_owner[owner].values():
            word_vocab[s.word] = vocab_of[owner]

    for owner in owners:
        if only_room is not None and owner != only_room:
            continue
        vocab = vocab_of[owner]
        scripts = [s for s in by_owner[owner].values() if s.word in texts]
        # references to shared scripts converted elsewhere name the first copy
        for s in scripts:
            s.refs = {alias.get(w, w) for w in s.refs}
            for w in s.refs:
                if word_vocab.get(w, vocab) != vocab:
                    uses_of[vocab].add(word_vocab[w])
        ordered, deferred = order(scripts)
        lines = files[vocab]
        title = 'the shared scripts (ids 0x80..)' if owner == 'builtin' else 'room $%02X' % owner
        lines += ['', '\\ ---- %s ' % title + '-' * (90 - len(title)), '']
        room_uses = defaultdict(set)
        for s in scripts:
            for kind, idx, n in s.room_words:
                room_uses[(kind, idx)].add(n)
        if room_uses:
            lines += room_stubs(owner, room_uses) + ['']
        for w in sorted(deferred):
            lines.append('defer %s' % w)
        for s in ordered:
            text = list(texts[s.word])
            if s.word in deferred:
                text[0] = ':noname   \\ %s (%s; deferred: used before it is defined)' % (s.word, '%08X' % s.addr)
                text[-1] = '; is %s' % s.word
                STATS['deferred (used before defined)'] += 1
            lines += text + ['']
        # the engine's tables
        for s in by_owner[owner].values():
            w = alias.get(s.word, s.word)
            if word_vocab.get(w, vocab) != vocab:
                uses_of[vocab].add(word_vocab[w])
            if owner == 'builtin':
                if s.name.startswith('act'):
                    lines.append("' %s $%s builtin-script!" % (w, s.name[3:]))
            elif s.name.startswith('act'):
                lines.append("' %s $%02X $%s action-script!" % (w, owner, s.name[3:]))
            else:
                lines.append("' %s $%02X %d room-script!" % (w, owner, PHASE_SLOT[s.name]))
    return files, uses_of


def header(vocab, uses):
    what = {'events.builtin': 'the shared event scripts (ids 0x80..) and the built-in ones',
            'events.other': "the event scripts of the rooms on none of the game's maps"}.get(
        vocab, "the event scripts of the rooms on the game's map %s (kMapRooms)" % vocab[-1])
    return ['\\ events/%s.fs - %s.' % (vocab.split('.')[1], what),
            '\\ Converted from the game\'s bytecode by tools/events2forth.py, once: edit by hand.',
            'IN: %s' % vocab,
            'USING: %s ;' % ' '.join(['events.words'] + sorted(uses - {vocab}))]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--stats', action='store_true')
    ap.add_argument('--room')
    a = ap.parse_args()
    only = int(a.room, 16) if a.room else None
    files, uses = convert(only)
    if a.room:
        for vocab, lines in files.items():
            print('\n'.join(lines))
    elif not a.stats:
        OUT.mkdir(parents=True, exist_ok=True)
        (OUT / 'words.fs').write_text('\n'.join(words_file()) + '\n')
        for vocab, lines in files.items():
            path = OUT / (vocab.split('.')[1] + '.fs')
            path.write_text('\n'.join(header(vocab, uses[vocab]) + lines) + '\n')
            print('wrote', path.relative_to(REPO), len(lines), 'lines')
    for k, v in sorted(STATS.items()):
        print('%-50s %d' % (k, v), file=sys.stderr)


if __name__ == '__main__':
    main()
