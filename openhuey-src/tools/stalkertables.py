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
(the ActionOffsets), his animations' sounds (+0x16AC), his knocks (+0x16B0), his chance roll (+0x1740) and rests (+0x1748). Each set is gathered into a kit (`debilitas-kit`, `daniella-kit`): a
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


def sound_num(x):   # a sound number, or -1 none
    return '$%X' % x if x >= 0 else '-1'


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
def ly(h, r):   # Lorenzo2's attack tables [hard][rage] (lorenzo.c sAttackTables2: the rage sets share some)
    base = 'Lorenzo2_Attack%d0_%02d' % (h, 0)
    names = []
    for i in range(17):
        own = 'Lorenzo2_Attack%d%d_%02d' % (h, r, i)
        names.append(own if (r == 0 or i not in (4, 5, 7, 14, 15)) else 'Lorenzo2_Attack%d0_%02d' % (h, i))
    return dan(names)


LY_TABLES = ly(0, 0) + ly(0, 1) + ly(1, 0) + ly(1, 1)
LY_COMMON = dict(steps=(0x422500, 0x422650), knocks=0x422650, tables=LY_TABLES, combos=(0x422A40, 12), gestures=(0x422A10, 6), chase=0x3AFB70, lunge=0x3AF4B0,
                 temper=(0x4230E0, 0x423990), pursue=(0x423020, 0x423070, 0x4238B0, 0x423900), throws=(9, 10),
                 grips=((1.1771, 9.8175), (1.1771, 9.8175), (0.9538, 13.3707), (0.9538, 13.3707),
                        (-1.1471, -2.5207), (-0.1481, -3.9775)))
K39_TABLES = (dan(['Kind39_Attack0_%02d' % i for i in range(17)]) * 2 +
              dan(['Kind39_Attack1_%02d' % i for i in range(17)]) * 2)
K39_COMMON = dict(steps=(0x444B50, 0x444CA0), knocks=0x444CA0, tables=K39_TABLES, combos=(0x445090, 12), gestures=(0x445060, 6), chase=0x3AFB70, lunge=0x3AF4B0,
                  temper=(0x445550, 0x445A00), pursue=(0x4454E0, 0x4454E0, 0x445990, 0x445990), throws=(9, 10),
                  grips=LY_COMMON['grips'])
def k37(h, a):   # kind 37's attack tables [hard][alerted] (riccardo.c sAttackTables2)
    names = ['Kind37_Attack%d%d_%02d' % (h, a, i) for i in range(14)] + ['Kind37_Attack%d0_14' % h]
    names += ['str_r_20', 'str_t_23' if a == 0 else 'str_t_24'] if h == 0 else ['str_r_21', 'str_t_25' if a == 0 else 'str_t_26']
    return dan(names)


def c_tables(array):   # an attack-table array of daniella.c, its names in order ([hard][rage][17] or [hard][17])
    src = open(os.path.join(root, 'src', 'game', 'daniella.c')).read()
    body = src[src.index('static u8 *const %s[' % array):]
    body = body[body.index('{'):body.index('};')]
    return dan(re.findall(r'\b(\w+)\b', body))


TS_TABLES = (dan(['TintStalker_Attack0_%02d' % i for i in range(15)] + ['str_r_8', 'str_t_13']) +
             dan(['TintStalker_Attack1_%02d' % i for i in range(15)] + ['str_r_9', 'str_t_14']))
DEB_GRIPS = ((0.9433, 11.2509), (0.9433, 11.2509), (2.1745, 15.0419), (2.1745, 15.0419),
             (-1.6117, -4.3418), (0.0182, -1.0008))   # (Debilitas_ActionOffsets; his other kinds' the same)
DAN_GRIPS = ((0.2576, 10.7031), (0.2576, 10.7031), (1.6432, 10.9092), (1.6432, 10.9092),
             (-0.1714, -3.1206), (0.4004, -2.9467))   # (Daniella_ActionOffsets; her later kinds' the same)
STALKERS = [
    # name, prefix, attack tables [hard][rage][17] (Debilitas: his rage the same), moves, combos,
    # gestures, chase, lunge, temper (normal, hard), base chase [hard][rage], throws, grip offsets
    dict(name='Debilitas', steps=(0x3AF340, 0x3AF4A0), knocks=0x3AF4A0, p='deb', tables=DEB_TABLES[:17] * 2 + DEB_TABLES[17:] * 2,
         moves=(0x3AF4D0, 15), combos=(0x3AF730, 18), gestures=(0x3AF6F0, 8), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x3AFB10, 0x3AFB10), pursue=(0x3AFA90, 0x3AFA90, 0x3AFA90, 0x3AFA90), throws=(13, 14),
         grips=DEB_GRIPS),
    # Debilitas's other kinds (debilitas2.c Debilitas2_Setup: kind 6; debilitas.c Debilitas3_Setup /
    # Kind27_Setup: kinds 7 and 27): his moves, gestures, combos and grips again, their own attack
    # tables (by the hard setting, no rage), base chase (+0x1730), temper, animations' sounds and
    # knocks; the chase and lunge tables are Debilitas's own chase's, which these never run
    dict(name='Debilitas6', steps=(0x41B6E0, 0x41B840), knocks=0x41B840, p='db6',
         tables=(lambda t: t[:17] * 2 + t[17:] * 2)(dan(['Debilitas2_Attack0_%02d' % i for i in range(16)] + ['str_t_11'] +
                                                        ['Debilitas2_Attack1_%02d' % i for i in range(16)] + ['str_t_12'])),
         moves=(0x41B860, 15), combos=(0x41BAC0, 18), gestures=(0x41BA80, 8), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x41BE80, 0x41BE80), pursue=(0x41BDF0, 0x41BDF0, 0x41C260, 0x41C260), throws=(13, 14),
         grips=DEB_GRIPS),
    dict(name='Debilitas7', steps=(0x413640, 0x4137A0), knocks=0x4137A0, p='db7',
         tables=(lambda t: t[:17] * 2 + t[17:] * 2)(dan(['Debilitas3_Attack0_%02d' % i for i in range(16)] + ['str_t_5'] +
                                                        ['Debilitas3_Attack1_%02d' % i for i in range(16)] + ['str_t_6'])),
         moves=(0x4137B0, 15), combos=(0x413A10, 18), gestures=(0x4139D0, 8), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x413E80, 0x413E80), pursue=(0x413E10, 0x413E10, 0x414220, 0x414220), throws=(13, 14),
         grips=DEB_GRIPS),
    dict(name='Kind27', steps=(0x42E5B0, 0x42E710), knocks=0x42E710, p='k27',
         tables=(lambda t: t[:17] * 2 + t[17:] * 2)(dan(['Kind27_Attack0_%02d' % i for i in range(16)] + ['str_t_15'] +
                                                        ['Kind27_Attack1_%02d' % i for i in range(16)] + ['str_t_16'])),
         moves=(0x42E720, 15), combos=(0x42E980, 18), gestures=(0x42E940, 8), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x42ED60, 0x42ED60), pursue=(0x42ECF0, 0x42ECF0, 0x42F110, 0x42F110), throws=(13, 14),
         grips=DEB_GRIPS),
    dict(name='Riccardo', steps=(0x4148E0, 0x414A30), knocks=0x414A30, p='ric', tables=RIC_TABLES[:17] * 2 + RIC_TABLES[17:] * 2,
         moves=(0x414A40, 18), combos=(0x414D00, 20), gestures=(0x414CD0, 6), chase=0x3AFB70, lunge=0x47AC38,
         temper=(0x4150D0, 0x4150D0), pursue=(0x415060, 0x415060, 0x4155E0, 0x4155E0), throws=(16, 17),
         grips=((-0.0389, 10.4049), (-0.0389, 10.4049), (0.7311, 13.251), (0.7311, 13.251),
                (-1.1471, -2.5207), (-0.328, -2.3929)),
         waits=0x415130, flurry=(0x4156C0, 0x4156E0), lunge_n=1),
    # Lorenzo: his temper with Hewie (+0x173C, 0x01990F90) is in memory the game fills - none here
    # (0: he never goes for Hewie); no throws of Hewie (AttackAnimA / B: the Pursuer's -1)
    dict(name='Lorenzo', steps=(0x41A630, 0x41A760), knocks=0x41A760, p='lor', tables=LOR_TABLES[:17] * 2 + LOR_TABLES[17:] * 2,
         moves=(0x41A770, 5), combos=(0x41A850, 8), gestures=(0x41A830, 4), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(None, None), pursue=(0x41ABD0, 0x41ABD0, 0x41AFC0, 0x41AFC0), throws=(-1, -1),
         grips=((-1.2219, -15.9398), (-1.2219, -15.9398), (0.2996, 14.3695), (0.2996, 14.3695),
                (-0.396, -1.3011), (-0.3379, -0.706))),
    # Lorenzo's other class (kind 10): his moves by the hard setting - a kit each
    dict(name='Lorenzo2', p='ly0', moves=(0x4226F0, 11), **LY_COMMON),
    dict(name='Lorenzo2H', p='ly1', moves=(0x422880, 11), **LY_COMMON),
    # kind 39 (lorenzo.c Kind39_*): Lorenzo's other class again with its own tables, no "rage"
    dict(name='Kind39', p='k39', moves=(0x444D40, 11), **K39_COMMON),
    dict(name='Kind39H', p='k39h', moves=(0x444ED0, 11), **K39_COMMON),
    # Riccardo's other class (kind 37; riccardo.c Kind37_*): his chase with its own tables; alerted
    # (struck enough: hurt) its alert tables and waits
    dict(name='Kind37', steps=(0x441890, 0x4419E0), knocks=None, p='k37', tables=k37(0, 0) + k37(0, 1) + k37(1, 0) + k37(1, 1),
         moves=(0x4419E0, 18), combos=(0x441CA0, 20), gestures=(0x441C70, 6), chase=0x3AFB70, lunge=0x47AF50,
         temper=(0x442230, 0x442230), pursue=(0x4421A0, 0x4421A0, 0x4421A0, 0x4421A0), throws=(16, 17),
         grips=((-0.0389, 10.4049), (-0.0389, 10.4049), (0.7311, 13.251), (0.7311, 13.251),
                (-1.1471, -2.5207), (-0.328, -2.3929)),
         waits=(0x442290, 0x4428C0, 0x4422B0, 0x4428E0), flurry=(0x4156C0, 0x4156E0), lunge_n=1),
    # the TintStalker (kind 23; tintstalker.c): the Pursuer's chase with its tables, Riccardo's look
    dict(name='TintStalker', steps=(0x42A380, 0x42A4D0), knocks=0x42A4D0, p='ts', tables=TS_TABLES[:17] * 2 + TS_TABLES[17:] * 2,
         moves=(0x42A4E0, 18), combos=(0x42A7A0, 20), gestures=(0x42A770, 6), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x42AB40, 0x42AB40), pursue=(0x42AAD0, 0x42AAD0, 0x42AAD0, 0x42AAD0), throws=(16, 17),
         grips=((-0.0389, 10.4049), (-0.0389, 10.4049), (0.7311, 13.251), (0.7311, 13.251),
                (-1.1471, -2.5207), (-0.328, -2.3929)), run={15: 0x42AAC0}),   # (TintStalker_AttackTable in the run)
    # kind 12 (lorenzo.c Kind12_*): unkillable, it seizes her seeing her; its temper with Hewie is in
    # memory the game fills (none), no throws, no grips of its own
    dict(name='Kind12', steps=(0x423BB0, 0x423CE0), knocks=0x423CE0, p='k12',
         tables=[0x423D90, 0x423DE0, 0x423DB0, 0x423DF8, 0x423E10, 0x423E38, 0x423E50, 0x423E70, 0x423E88,
                 0x423E98, 0x423EB0, 0x423EC8, 0x423EE0, 0x423F00, 0x423F38, 0x423F48, 0x423F20] * 2 +
                [0x423FE0, 0x424040, 0x424010, 0x424060, 0x424080, 0x4240B0, 0x4240C0, 0x4240F0, 0x424108,
                 0x424118, 0x424130, 0x424158, 0x424170, 0x424190, 0x4241C8, 0x4241D8, 0x4241B0] * 2,
         moves=(0x423CF0, 3), combos=(0x423D80, 4), gestures=(0x423D60, 4), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(None, None), pursue=(0x423F60, 0x423F60, 0x4241F0, 0x4241F0), throws=(-1, -1),
         grips=((0.0, 0.0),) * 6),
    # Daniella's later kinds (daniella.c Kind34_* / Kind35_* / Kind36_*): her moves, gestures and
    # combos of their own, her grips; 34 has her rage, 35 and 36 none (their sets by the hard
    # setting only)
    dict(name='Kind34', steps=(0x43B700, 0x43B840), knocks=0x43B840, p='k34', tables=c_tables('sAttackTables1'),
         moves=(0x43B860, 13), combos=(0x43BA70, 20), gestures=(0x43BA40, 6), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x43C2F0, 0x43CCB0), pursue=(0x43C210, 0x43C260, 0x43CBD0, 0x43CC20), throws=(11, 12),
         grips=DAN_GRIPS, run={15: 0x43C200, 14: 0x43C1F0}),   # (Kind34_AttackTable in the run: Kind34_Attack01_15 / _14)
    dict(name='Kind35', steps=(0x43CDE0, 0x43CF20), knocks=None, p='k35', tables=(lambda t: t[:17] * 2 + t[17:] * 2)(c_tables('sAttackTables2')),
         moves=(0x43CF20, 13), combos=(0x43D130, 20), gestures=(0x43D100, 6), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x43D650, 0x43DB50), pursue=(0x43D5E0, 0x43D5E0, 0x43DAE0, 0x43DAE0), throws=(11, 12),
         grips=DAN_GRIPS),
    dict(name='Kind36', steps=(0x43DC70, 0x43DDB0), knocks=None, p='k36', tables=(lambda t: t[:17] * 2 + t[17:] * 2)(c_tables('sAttackTables3')),
         moves=(0x43DDB0, 13), combos=(0x43DFC0, 20), gestures=(0x43DF90, 6), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x43E4E0, 0x43EA00), pursue=(0x43E470, 0x43E470, 0x43E990, 0x43E990), throws=(11, 12),
         grips=DAN_GRIPS),
    dict(name='Daniella', steps=(0x3D73F0, 0x3D7530), knocks=0x3D7530, p='dan', tables=DAN_TABLES,
         moves=(0x3D7550, 13), combos=(0x3D7760, 18), gestures=(0x3D7730, 6), chase=0x3AFB70, lunge=0x3AF4B0,
         temper=(0x3D7F40, 0x3D8890), pursue=(0x3D7E60, 0x3D7EB0, 0x3D87B0, 0x3D8800), throws=(11, 12),
         grips=DAN_GRIPS),
]

# each one's chance roll (+0x1740: {chance, rise} - Hewie down, his blow struck Hewie, else, struck
# by someone: Pursuer_ChanceRoll) and his rests (+0x1748: after a blow that struck, at least after
# one that missed, after a hold that caught), [normal, raging / alerted, hard, hard raging] as his
# Setup / SetRage / Kind37_Update set them. None: memory nothing writes (Lorenzo2's hard rests,
# 0x019910C8 / 0x019910D8) - zeros
ROLLS = {
    'Debilitas': ((0x3AFAF0,) * 4, (0x3AFB58,) * 4),
    'Debilitas6': ((0x41BE60, 0x41BE60, 0x41C2B0, 0x41C2B0), (0x41BEC8,) * 4),
    'Debilitas7': ((0x413E60, 0x413E60, 0x414270, 0x414270), (0x413EC8,) * 4),
    'Kind27': ((0x42ED40, 0x42ED40, 0x42F160, 0x42F160), (0x42EDA8,) * 4),
    'Riccardo': ((0x4150B0, 0x4150B0, 0x415630, 0x415630), (0x415118, 0x415118, 0x415650, 0x415650)),
    'Lorenzo': ((0x41AC20,) * 4, (0x41AC40, 0x41AC40, 0x41B008, 0x41B008)),
    'Lorenzo2': ((0x4230C0, 0x4230C0, 0x423950, 0x423970), (0x423128, 0x423128, None, None)),
    'Kind39': ((0x445530, 0x445530, 0x4459E0, 0x4459E0), (0x445598, 0x445598, 0x445A48, 0x445A48)),
    'Kind37': ((0x4421F0, 0x442210, 0x442870, 0x442890), (0x442278, 0x442278, 0x442278, 0x4428B0)),
    'TintStalker': ((0x42AB20,) * 4, (0x42AB88,) * 4),
    'Kind12': ((0x423FB0,) * 4, (0x423FD0, 0x423FD0, 0x424238, 0x424238)),
    'Kind34': ((0x43C2B0, 0x43C2D0, 0x43CC70, 0x43CC90), (0x43C338, 0x43C348, 0x43CCF8, 0x43CD08)),
    'Kind35': ((0x43D630, 0x43D630, 0x43DB30, 0x43DB30), (0x43D698, 0x43D698, 0x43DB98, 0x43DB98)),
    'Kind36': ((0x43E4C0, 0x43E4C0, 0x43E9E0, 0x43E9E0), (0x43E528, 0x43E528, 0x43EA48, 0x43EA48)),
    'Daniella': ((0x3D7F00, 0x3D7F20, 0x3D8850, 0x3D8870), (0x3D7F88, 0x3D7F98, 0x3D88D8, 0x3D88E8)),
}
ROLLS['Lorenzo2H'] = ROLLS['Lorenzo2']
ROLLS['Kind39H'] = ROLLS['Kind39']

out = ['\\ stalker/tables.fs - generated by tools/stalkertables.py: the stalkers\' tables from the',
       '\\ executable (src/game/debilitas.c, daniella.c), a set each, and their kits. Rows: kind,',
       '\\ argument, running percentage (0..100).',
       'IN: stalker.tables', 'USING: engine game-state common facts messages stalker.state ;', '',
       '\\ a row of a table: what he does (an action, or $1000.. his own), its argument, the running',
       '\\ percentage it is picked up to',
       'begin-structure /row  field: r.act  field: r.arg  ffield: r.upto  end-structure',
       '\\ a move: animation, bone, second bone (-1 none), reach, kind (her request: a blow, a hard one,',
       '\\ a seize, a hold), damage, threat, stumble chance, turns with it, cry (-1 none), unshaken (a',
       '\\ shove doesn\'t make him flinch during it)',
       'begin-structure /move',
       '    field: m.anim  field: m.bone  field: m.bone2  ffield: m.reach  field: m.kind  field: m.damage',
       '    ffield: m.threat  ffield: m.stumble  field: m.turns  field: m.cry  field: m.unshaken',
       'end-structure', '']
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
    for t in s.get('run', {}).values():   # (the mini-game's run: its own for some situations)
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
    out.append('create %s-run  ' % p + ' '.join('%d , %s ,' % (k, names[t]) for k, t in s.get('run', {}).items()) +
               ' -1 ,   \\ (the run: situation, table)')
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
    if 'waits' in s:   # (his own chase's waits, +0x1824: walking, stalking, walking seen, stalking far, comings-on,
        # the chance - by the hard setting, and alerted: kind 37's)
        w = s['waits'] if isinstance(s['waits'], tuple) else (s['waits'],) * 4
        for k, t in enumerate(w):
            out.append('create %s-waits-%d  ' % (p, k) + ' '.join('%d ,' % v for v in rd(t, '<5i')) + ' %s f,' % f(rd(t + 20, '<f')[0]))
        for k, t in enumerate(s['flurry']):   # (his flurry's blows, 1..7: running chances)
            out.append('create %s-flurry-%d  ' % (p, k) + ' '.join('%s f,' % f(v) for v in rd(t, '<6f')))
    a, z = s['steps']   # (his animations' sounds at their two sound keys, by his motions' order; -1 none)
    out.append('create %s-anim-sounds  %d ,  ' % (p, (z - a) // 4) + '  '.join('%s , %s ,' % tuple(sound_num(x) for x in rd(a + 4 * i, '<hh')) for i in range((z - a) // 4)))
    if s['knocks']:   # (his knocks at a shut door into her room: sound, running chance)
        rows = []
        while True:
            snd, pc = rd(s['knocks'] + 8 * len(rows), '<hxxf')
            rows.append('$%X , %s f,' % (snd, f(pc)))
            if pc >= 100:
                break
        out.append('create %s-knocks  ' % p + '  '.join(rows))
    else:
        out.append('create %s-knocks  $21 , 100e f,   \\ (none of his own: the Pursuer\'s)' % p)
    rolls, rests = ROLLS[s['name']]
    made = {}
    for t in rolls:
        if t not in made:
            made[t] = '%s-rolls-%X' % (p, t)
            out.append('create %s  ' % made[t] + '  '.join('%s f, %s f,' % (f(a), f(b)) for a, b in zip(*[iter(rd(t, '<8f'))] * 2)))
    out.append('create %s-rolls  ' % p + ' '.join('%s ,' % made[t] for t in rolls) + '   \\ [hard][rage]')
    for t in rests:
        if t not in made:
            made[t] = '%s-rests-%X' % (p, t if t else 0)
            out.append('create %s  ' % made[t] + ' '.join('%d ,' % v for v in (rd(t, '<3i') if t else (0, 0, 0))))
    out.append('create %s-rests  ' % p + ' '.join('%s ,' % made[t] for t in rests) + '   \\ [hard][rage]')
    out.append('create %s-kit  %s-tables , %s-moves , %s-combos , %s-gestures , %s-chase-frames , %s-chase-chances ,'
               % ((s['name'].lower(),) + (p,) * 6))
    out.append('    %s-lunge-chances , %s-temper-0 , %s-temper-1 , %s-pursue-0 , %s-pursue-1 , %s-pursue-2 , %s-pursue-3 ,'
               % ((p,) * 7))
    out.append('    %d , %d , %s-grips ,' % (s['throws'][0], s['throws'][1], p))
    out.append(('    %s-waits-0 , %s-flurry-0 , %s-flurry-1 , %s-waits-1 , %s-waits-2 , %s-waits-3 ,' % ((p,) * 6))
               if 'waits' in s else '    0 , 0 , 0 , 0 , 0 , 0 ,')
    out.append('    %s-knocks , %s-anim-sounds , %s-run , %s-rolls , %s-rests ,' % (p, p, p, p, p))
    out.append('')
out += [
    '\\ ---- a kit: where his tables are (his `kit` field points at his). The tables by situation: 17,',
    '\\ his rage\'s 17, the hard setting\'s 17 and 17 ----',
    'begin-structure /kit',
    '    field: k.tables  field: k.moves  field: k.combos  field: k.gestures',
    '    field: k.chase-frames  field: k.chase-chances  field: k.lunge-chances  field: k.temper  field: k.temper-hard',
    '    field: k.pursue  field: k.pursue-rage  field: k.pursue-hard  field: k.pursue-hard-rage',
    '    field: k.throw-a  field: k.throw-b  field: k.grips',
    '    field: k.waits  field: k.flurry  field: k.flurry-hard  field: k.waits-hard  field: k.waits-hurt  field: k.waits-hard-hurt',
    '    field: k.knocks  field: k.anim-sounds  field: k.run  field: k.rolls  field: k.rests',
    'end-structure',
    ': my-kit ( -- kit )  kit @ ;',
    ': hard-set? ( -- flag )  hard-mode? ;',
    '\\ his table for a situation (raging or alerted: his rage\'s; in the mini-game\'s run, his own for some)',
    ': run-table ( situation -- addr | 0 )',
    '    hewie-played? 0= if  drop 0 exit  then  my-kit k.run @',
    '    begin  dup @ dup -1 <> while  2 pick = if  nip cell+ @ exit  then  2 cells +  repeat  2drop drop 0 ;',
    ': table ( situation hard? -- addr )',
    '    over run-table ?dup if  nip nip exit  then',
    '    if  34  else  0  then  raging @ alt-tables @ or if  17 +  then  +  cells my-kit k.tables @ + @ ;',
    ': moves ( -- addr )  my-kit k.moves @ ;',
    ': combos ( -- addr )  my-kit k.combos @ ;',
    ': gestures ( -- addr )  my-kit k.gestures @ ;',
    ': chase-frames ( -- addr )  my-kit k.chase-frames @ ;',
    ': chase-chances ( -- addr )  my-kit k.chase-chances @ ;',
    ': lunge-chances ( -- addr )  my-kit k.lunge-chances @ ;',
    ': temper-table ( -- addr )  my-kit hard-set? if  k.temper-hard  else  k.temper  then  @ ;',
    '\\ the base chase\'s row for the panic\'s stage: the chance to walk at her, frames walking, stalking',
    ': pursue-row ( stage -- addr )',
    '    my-kit hard-set? if  k.pursue-hard  else  k.pursue  then  raging @ if  cell+  then  @  swap 0 5 clamp 3 cells * + ;',
    ': throw-moves ( -- a b )  my-kit k.throw-a @  my-kit k.throw-b @ ;',
    ': grip-offsets ( -- addr )  my-kit k.grips @ ;',
    '\\ his own chase\'s waits (Riccardo: +0x1824) and his flurry\'s chances, by the hard setting (0: none)',
    ': waits ( -- addr )   \\ (alerted - hurt - kind 37\'s own)',
    '    my-kit  cond @ hurt-cond = if  hard-set? if  k.waits-hard-hurt  else  k.waits-hurt  then',
    '    else  hard-set? if  k.waits-hard  else  k.waits  then  then  @ ;',
    ': flurry-chances ( -- addr )  my-kit hard-set? if  k.flurry-hard  else  k.flurry  then  @ ;',
    '\\ his knocks at a shut door (sound, running chance) and his animations\' sounds (a count, then',
    '\\ the first and second sound key\'s by his motions\' order)',
    ': knocks ( -- addr )  my-kit k.knocks @ ;',
    ': anim-sounds ( -- addr )  my-kit k.anim-sounds @ ;',
    ': move-of ( n -- addr )  /move * moves + ;',
    '\\ his chance roll\'s table (+0x1740: {chance, rise} - Hewie down, his blow struck Hewie, else, struck',
    '\\ by someone) and his rests (+0x1748: frames after a blow that struck, at least after one that',
    '\\ missed, after a hold that caught), by the hard setting and his rage (or kind 37 alerted)',
    ': by-stance ( addr -- addr\' )  hard-set? if  2 cells +  then  raging @ alt-tables @ or if  cell+  then  @ ;',
    ': rolls ( -- addr )  my-kit k.rolls @ by-stance ;',
    ': rests ( -- addr )  my-kit k.rests @ by-stance ;',
    ': rest-struck ( -- n )  rests @ ;   : rest-missed ( -- n )  rests cell+ @ ;   : rest-held ( -- n )  rests 2 cells + @ ;',
]
open(os.path.join(here, '..', 'scripts', 'stalker', 'tables.fs'), 'w').write('\n'.join(out) + '\n')
print('written')
