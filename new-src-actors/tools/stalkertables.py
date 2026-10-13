#!/usr/bin/env python3
"""The stalkers' tables read from the executable into scripts/stalker/tables.fs, a set for each
(src/game/debilitas.c Debilitas_Setup / Debilitas_AttackTable, src/game/daniella.c
Daniella_Setup / Daniella_AttackTable / Daniella_SetRage): what he does in each situation (17
tables, by the hard setting and his rage: rows of a kind, an argument and a running
percentage), his combos (4 move numbers, -1 ends), his moves (0x24 each: animation, the
striking bone and a second, reach, kind, damage, threat, stumble chance, turning with it, its
cry), his gestures (animation, a flag), Debilitas's chase table (frames and chances) and the
chances of a lunge by how near she is, the base chase's table by the panic's stage (+0x1730:
the chance to walk at her, frames walking, frames stalking), his temper with Hewie (+0x173C),
his throws off Hewie (Pursuer_ShakeOffEntry's moves) and Hewie's grip spots by him
(the ActionOffsets). Each set is gathered into a kit (`debilitas-kit`, `daniella-kit`): a
stalker's `kit` field points at his, and the words below read through it.

usage: stalkertables.py [repo root]"""
import os
import re
import struct
import sys

here = os.path.dirname(os.path.abspath(__file__))
root = sys.argv[1] if len(sys.argv) > 1 else os.path.join(here, '..', '..')
elf = open(os.path.join(root, 'baserom', 'SLUS_210.75'), 'rb').read()
ph, = struct.unpack_from('<I', elf, 0x1C)
n, = struct.unpack_from('<H', elf, 0x2C)
segs = [struct.unpack_from('<5I', elf, ph + i * 32)[1:] for i in range(n)]
syms = {}
for line in open(os.path.join(root, 'config', 'symbol_addrs.txt')):
    m = re.match(r'(\w+) = 0x([0-9A-Fa-f]+);', line)
    if m:
        syms[m.group(1)] = int(m.group(2), 16)


def rd(va, fmt):
    for off, vaddr, _, size in segs:
        if vaddr <= va < vaddr + size:
            return struct.unpack_from(fmt, elf, off + va - vaddr)
    raise ValueError(hex(va))


def f(x):
    return ('%ge' % x) if 'e' not in ('%g' % x) else '%g' % x


DEB_TABLES = [0x3AF780, 0x3AF800, 0x3AF7C0, 0x3AF840, 0x3AF880, 0x3AF8A0, 0x3AF8C0, 0x3AF900, 0x3AF930,
              0x3AF978, 0x3AF990, 0x3AF9D0, 0x3AF9E0, 0x3AFA10, 0x3AFA70, 0x3AFA80, 0x3AFA40,
              0x3AFBA0, 0x3AFC40, 0x3AFBF0, 0x3AFC80, 0x3AFCB0, 0x3AFCF0, 0x3AFD10, 0x3AFD50, 0x3AFD80,
              0x3AFDC0, 0x3AFDD0, 0x3AFE00, 0x3AFE10, 0x3AFE50, 0x3AFED0, 0x3AFEE0, 0x3AFEA0]


def dan(names):
    return [syms[x] for x in names]


DAN_TABLES = (dan(['Daniella_Attack00_%02d' % i for i in range(15)] + ['str_r_2', 'str_t_3']) +
              dan(['Daniella_Attack01_%02d' % i for i in range(17)]) +
              dan(['Daniella_Attack10_%02d' % i for i in range(14)] + ['str_r_3', 'str_r_4', 'str_t_4']) +
              dan(['Daniella_Attack11_%02d' % i for i in range(17)]))
RIC_TABLES = (dan(['Riccardo_Attack0_%02d' % i for i in range(15)] + ['str_r_5', 'str_t_7']) +
              dan(['Riccardo_Attack1_%02d' % i for i in range(15)] + ['str_r_6', 'str_t_8']))
LOR_TABLES = (dan(['Lorenzo_Attack0_%02d' % i for i in range(16)] + ['str_t_9']) +
              dan(['Lorenzo_Attack1_%02d' % i for i in range(16)] + ['str_t_10']))
STALKERS = [
    # name, prefix, attack tables [hard][rage][17] (Debilitas: his rage the same), moves, combos,
    # gestures, chase, lunge, temper (normal, hard), base chase [hard][rage], throws, grip offsets
    dict(name='Debilitas', p='deb', tables=DEB_TABLES[:17] * 2 + DEB_TABLES[17:] * 2,
         moves=(0x3AF4D0, 15), combos=(0x3AF730, 16), gestures=(0x3AF6F0, 8), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x3AFB10, 0x3AFB10), pursue=(0x3AFA90, 0x3AFA90, 0x3AFA90, 0x3AFA90), throws=(13, 14),
         grips=((0.9433, 11.2509), (0.9433, 11.2509), (2.1745, 15.0419), (2.1745, 15.0419),
                (-1.6117, -4.3418), (0.0182, -1.0008))),
    dict(name='Riccardo', p='ric', tables=RIC_TABLES[:17] * 2 + RIC_TABLES[17:] * 2,
         moves=(0x414A40, 18), combos=(0x414D00, 20), gestures=(0x414CD0, 6), chase=0x3AFB70, lunge=0x47AC38,
         temper=(0x4150D0, 0x4150D0), pursue=(0x415060, 0x415060, 0x4155E0, 0x4155E0), throws=(16, 17),
         grips=((-0.0389, 10.4049), (-0.0389, 10.4049), (0.7311, 13.251), (0.7311, 13.251),
                (-1.1471, -2.5207), (-0.328, -2.3929)),
         waits=0x415130, flurry=(0x4156C0, 0x4156E0), lunge_n=1),
    # Lorenzo: his temper with Hewie (+0x173C, 0x01990F90) is in memory the game fills - none here
    # (0: he never goes for Hewie); no throws of Hewie (AttackAnimA / B: the Pursuer's -1)
    dict(name='Lorenzo', p='lor', tables=LOR_TABLES[:17] * 2 + LOR_TABLES[17:] * 2,
         moves=(0x41A770, 5), combos=(0x41A850, 8), gestures=(0x41A830, 4), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(None, None), pursue=(0x41ABD0, 0x41ABD0, 0x41AFC0, 0x41AFC0), throws=(-1, -1),
         grips=((-1.2219, -15.9398), (-1.2219, -15.9398), (0.2996, 14.3695), (0.2996, 14.3695),
                (-0.396, -1.3011), (-0.3379, -0.706))),
    dict(name='Daniella', p='dan', tables=DAN_TABLES,
         moves=(0x3D7550, 13), combos=(0x3D7760, 18), gestures=(0x3D7730, 6), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x3D7F40, 0x3D8890), pursue=(0x3D7E60, 0x3D7EB0, 0x3D87B0, 0x3D8800), throws=(11, 12),
         grips=((0.2576, 10.7031), (0.2576, 10.7031), (1.6432, 10.9092), (1.6432, 10.9092),
                (-0.1714, -3.1206), (0.4004, -2.9467))),
]

out = ['\\ stalker/tables.fs - generated by tools/stalkertables.py: the stalkers\' tables from the',
       '\\ executable (src/game/debilitas.c, daniella.c), a set each, and their kits. Rows: kind,',
       '\\ argument, running percentage (0..100).',
       'IN: stalker.tables', 'USING: engine game-state common stalker.state ;', '',
       '3 cells constant /row   \\ (kind, argument, percentage: a cell each)',
       '\\ a move: animation, bone, second bone (-1 none), reach, kind (2 a blow, 4 a hard one, 3 a',
       '\\ seize, 6 a hold), damage, threat, stumble chance, turns with it, cry (-1 none), unshaken (a',
       '\\ shove doesn\'t make him flinch during it)',
       '11 cells constant /move', '']
for s in STALKERS:
    p = s['p']
    out.append('\\ ==== %s ====' % s['name'])
    names = {}
    for i, t in enumerate(s['tables']):
        if t in names:
            continue
        rows = []
        while True:
            k, a, pc = rd(t + 12 * len(rows), '<iif')
            rows.append('$%X , %d , %s f,' % (k, a, f(pc)))
            if pc >= 100 or len(rows) >= 12:
                break
        names[t] = '%s-table-%X' % (p, t)
        out.append('create %s  %s' % (names[t], '  '.join(rows)))
    out.append('create %s-tables  ' % p + ' '.join('%s ,' % names[t] for t in s['tables']) +
               '   \\ [hard][rage][situation]')
    va, nm = s['moves']
    rows = []
    for m in range(nm):
        b = va + 0x24 * m
        anim, bone, bone2, reach = rd(b, '<iiif')
        kind, = rd(b + 0x10, '<B')
        dmg, = rd(b + 0x12, '<h')
        thr, stum = rd(b + 0x14, '<ff')
        turn, held = rd(b + 0x1C, '<BB')
        cry, = rd(b + 0x20, '<i')
        rows.append('    $%X , $%X , %d , %s f, %d , %d , %s f, %s f, %d , %s ,' % (
            anim, bone, bone2, f(reach), kind, dmg, f(thr), f(stum), turn, ('$%X' % cry) if cry >= 0 else '-1') +
            ' %d ,' % held)
    out.append('create %s-moves\n' % p + '\n'.join(rows))
    va, nc = s['combos']
    out.append('create %s-combos  ' % p + '  '.join(' '.join('%d ,' % x for x in rd(va + 4 * c, '<4b')) for c in range(nc)))
    va, ng = s['gestures']
    out.append('create %s-gestures  ' % p + '  '.join('$%X , %d ,' % rd(va + 8 * g, '<iI') for g in range(ng)))
    out.append('create %s-chase-frames  ' % p + ' '.join('%d ,' % rd(s['chase'] + 4 * i, '<i')[0] for i in range(9)))
    out.append('create %s-chase-chances  ' % p + ' '.join('%s f,' % f(rd(s['chase'] + 0x24 + 4 * i, '<f')[0]) for i in range(3)))
    ln = s.get('lunge_n', 3)   # (Riccardo's table has one entry: the rest never met)
    out.append('create %s-lunge-chances  ' % p + '  '.join('%s f, %d ,' % (f(a), b) for a, b in
               [rd(s['lunge'] + 8 * i, '<fi') for i in range(ln)] + [(-1.0, 0)] * (3 - ln)))
    for k, t in enumerate(s['temper']):
        out.append('create %s-temper-%d  ' % (p, k) + ' '.join('%d ,' % v for v in (rd(t, '<18i') if t else [0] * 18)))
    for k, t in enumerate(s['pursue']):
        out.append('create %s-pursue-%d  ' % (p, k) + '  '.join('%s f, %d , %d ,' % (f(a), b, c) for a, b, c in (rd(t + 12 * i, '<fii') for i in range(6))))
    out.append('create %s-grips  ' % p + '  '.join('%se f, %se f,' % (repr(x), repr(z)) for x, z in s['grips']))
    if 'waits' in s:   # (his own chase's waits, +0x1824: back off, hold off, back off seen, hold off far, comings-on, the chance)
        out.append('create %s-waits  ' % p + ' '.join('%d ,' % v for v in rd(s['waits'], '<5i')) + ' %s f,' % f(rd(s['waits'] + 20, '<f')[0]))
        for k, t in enumerate(s['flurry']):   # (his flurry's blows, 1..7: running chances)
            out.append('create %s-flurry-%d  ' % (p, k) + ' '.join('%s f,' % f(v) for v in rd(t, '<6f')))
    out.append('create %s-kit  %s-tables , %s-moves , %s-combos , %s-gestures , %s-chase-frames , %s-chase-chances ,'
               % ((s['name'].lower(),) + (p,) * 6))
    out.append('    %s-lunge-chances , %s-temper-0 , %s-temper-1 , %s-pursue-0 , %s-pursue-1 , %s-pursue-2 , %s-pursue-3 ,'
               % ((p,) * 7))
    out.append('    %d , %d , %s-grips ,' % (s['throws'][0], s['throws'][1], p))
    out.append(('    %s-waits , %s-flurry-0 , %s-flurry-1 ,' % (p, p, p)) if 'waits' in s else '    0 , 0 , 0 ,')
    out.append('')
out += [
    '\\ ---- through his kit (his `kit` field) ----',
    ': kit# ( n -- x )  cells kit @ + @ ;',
    ': hard-set? ( -- flag )  progress pr.vars $27 + c@ 1 = ;',
    ': table ( situation hard? -- addr )   \\ (his rage: the second set of 17)',
    '    if  34  else  0  then  raging @ if  17 +  then  +  cells 0 kit# + @ ;',
    ': moves ( -- addr )  1 kit# ;',
    ': combos ( -- addr )  2 kit# ;',
    ': gestures ( -- addr )  3 kit# ;',
    ': chase-frames ( -- addr )  4 kit# ;',
    ': chase-chances ( -- addr )  5 kit# ;',
    ': lunge-chances ( -- addr )  6 kit# ;',
    ': temper-table ( -- addr )  hard-set? if  8  else  7  then  kit# ;',
    '\\ the base chase\'s row for the panic\'s stage: the chance to walk at her, frames walking, stalking',
    ': pursue-row ( stage -- addr )',
    '    hard-set? if  2  else  0  then  raging @ if  1+  then  9 + kit#  swap 0 5 clamp 3 cells * + ;',
    ': throw-moves ( -- a b )  13 kit#  14 kit# ;',
    ': grip-offsets ( -- addr )  15 kit# ;',
    '\\ his own chase\'s waits (Riccardo: +0x1824) and his flurry\'s chances, by the hard setting (0: none)',
    ': waits ( -- addr )  16 kit# ;',
    ': flurry-chances ( -- addr )  hard-set? if  18  else  17  then  kit# ;',
]
open(os.path.join(here, '..', 'scripts', 'stalker', 'tables.fs'), 'w').write('\n'.join(out) + '\n')
print('written')
