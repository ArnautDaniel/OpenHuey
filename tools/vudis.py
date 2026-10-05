#!/usr/bin/env python3
"""Disassemble a PS2 VU1 microprogram from the game's ELF.

    tools/vudis.py ADDR          walk the DMA chain at ADDR, disassemble each MPG upload
    tools/vudis.py --mpg ADDR N  disassemble N instructions at ADDR (VU address 0)

Used to read what the original's VU1 programs compute (lighting, skinning), so the OpenGL path
can do the same. Each line: VU address, upper instruction, lower instruction."""
import struct
import sys

ELF = '../Haunting Ground (USA)/SLUS_210.75'


def load():
    f = open(ELF, 'rb').read()
    phoff = struct.unpack_from('<I', f, 28)[0]
    phsz, phn = struct.unpack_from('<HH', f, 42)
    segs = []
    for i in range(phn):
        t, off, va, pa, fs, ms = struct.unpack_from('<IIIIII', f, phoff + i * phsz)
        if t == 1:
            segs.append((va, off, fs))

    def rd(a, n):
        for va, off, fs in segs:
            if va <= a < va + fs:
                return f[off + a - va: off + a - va + n]
        raise ValueError(hex(a))
    return rd


DEST = lambda d: ''.join(c for c, b in zip('xyzw', (8, 4, 2, 1)) if d & b)
BC = 'xyzw'
F = lambda r: 'vf%02d' % r
I = lambda r: 'vi%02d' % r


def upper(w):
    flags = ''.join(c for c, b in zip('IEMDT', (31, 30, 29, 28, 27)) if w >> b & 1)
    d = DEST(w >> 21 & 15)
    ft, fs, fd = w >> 16 & 31, w >> 11 & 31, w >> 6 & 31
    op = w & 0x3F
    bc = BC[w & 3]
    s = None
    names = {0: 'add', 1: 'sub', 2: 'madd', 3: 'msub', 4: 'max', 5: 'mini', 6: 'mul'}
    if op < 0x1C:
        s = '%s%s.%s %s, %s, %s%s' % (names[op >> 2], bc, d, F(fd), F(fs), F(ft), bc)
    elif op < 0x3C:
        tab = {0x1C: ('mulq', 'q'), 0x1D: ('maxi', 'i'), 0x1E: ('muli', 'i'), 0x1F: ('minii', 'i'),
               0x20: ('addq', 'q'), 0x21: ('maddq', 'q'), 0x22: ('addi', 'i'), 0x23: ('maddi', 'i'),
               0x24: ('subq', 'q'), 0x25: ('msubq', 'q'), 0x26: ('subi', 'i'), 0x27: ('msubi', 'i')}
        tab3 = {0x28: 'add', 0x29: 'madd', 0x2A: 'mul', 0x2B: 'max', 0x2C: 'sub', 0x2D: 'msub', 0x2E: 'opmsub',
                0x2F: 'mini'}
        if op in tab:
            s = '%s.%s %s, %s, %s' % (tab[op][0], d, F(fd), F(fs), tab[op][1])
        elif op in tab3:
            s = '%s.%s %s, %s, %s' % (tab3[op], d, F(fd), F(fs), F(ft))
        else:
            s = '?upper %08x' % w
    else:
        k = (w >> 4 & 0x7C) | (w & 3)
        acc = {0: 'adda', 1: 'suba', 2: 'madda', 3: 'msuba', 6: 'mula'}
        if k < 0x10 or 0x18 <= k < 0x1C:
            s = '%s%s.%s ACC, %s, %s%s' % (acc[k >> 2], BC[k & 3], d, F(fs), F(ft), BC[k & 3])
        elif 0x10 <= k < 0x14:
            s = 'itof%d.%s %s, %s' % ((0, 4, 12, 15)[k & 3], d, F(ft), F(fs))
        elif 0x14 <= k < 0x18:
            s = 'ftoi%d.%s %s, %s' % ((0, 4, 12, 15)[k & 3], d, F(ft), F(fs))
        else:
            tab = {0x1C: 'mulaq.%s ACC, %s, q', 0x1D: 'abs.%s %s, %s', 0x1E: 'mulai.%s ACC, %s, i',
                   0x1F: 'clipw.xyz %s, %sw', 0x20: 'addaq.%s ACC, %s, q', 0x21: 'maddaq.%s ACC, %s, q',
                   0x22: 'addai.%s ACC, %s, i', 0x23: 'maddai.%s ACC, %s, i', 0x24: 'subaq.%s ACC, %s, q',
                   0x25: 'msubaq.%s ACC, %s, q', 0x26: 'subai.%s ACC, %s, i', 0x27: 'msubai.%s ACC, %s, i',
                   0x28: 'adda.%s ACC, %s, %s', 0x29: 'madda.%s ACC, %s, %s', 0x2A: 'mula.%s ACC, %s, %s',
                   0x2C: 'suba.%s ACC, %s, %s', 0x2D: 'msuba.%s ACC, %s, %s', 0x2E: 'opmula.xyz ACC, %s, %s',
                   0x2F: 'nop'}
            t = tab.get(k)
            if t is None:
                s = '?upper %08x' % w
            elif k == 0x2F:
                s = 'nop'
            elif k == 0x1F:
                s = t % (F(fs), F(ft))
            elif k == 0x2E:
                s = t % (F(fs), F(ft))
            elif k == 0x1D:
                s = t % (d, F(ft), F(fs))
            elif t.count('%s') == 3:
                s = t % (d, F(fs), F(ft))
            else:
                s = t % (d, F(fs))
    return s + (' [' + flags + ']' if flags else '')


def simm(v, bits):
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def lower(w, pc):
    op = w >> 25
    d = DEST(w >> 21 & 15)
    ft, fs, fd = w >> 16 & 31, w >> 11 & 31, w >> 6 & 31
    imm11 = simm(w & 0x7FF, 11)
    imm15 = (w & 0x7FF) | (w >> 21 & 15) << 11
    if op == 0x40:
        sub = w & 0x3F
        if sub < 0x3C:
            tab = {0x30: 'iadd', 0x31: 'isub', 0x34: 'iand', 0x35: 'ior'}
            if sub in tab:
                return '%s %s, %s, %s' % (tab[sub], I(fd), I(fs), I(ft))
            if sub == 0x32:
                return 'iaddi %s, %s, %d' % (I(ft), I(fs), simm(fd, 5))
            return '?lower %08x' % w
        k = (w >> 4 & 0x7C) | (w & 3)
        fsf, ftf = BC[w >> 21 & 3], BC[w >> 23 & 3]
        tab = {
            0x30: 'move.%s %s, %s' % (d, F(ft), F(fs)), 0x31: 'mr32.%s %s, %s' % (d, F(ft), F(fs)),
            0x34: 'lqi.%s %s, (%s++)' % (d, F(ft), I(fs)), 0x35: 'sqi.%s %s, (%s++)' % (d, F(fs), I(ft)),
            0x36: 'lqd.%s %s, (--%s)' % (d, F(ft), I(fs)), 0x37: 'sqd.%s %s, (--%s)' % (d, F(fs), I(ft)),
            0x38: 'div q, %s%s, %s%s' % (F(fs), fsf, F(ft), ftf), 0x39: 'sqrt q, %s%s' % (F(ft), ftf),
            0x3A: 'rsqrt q, %s%s, %s%s' % (F(fs), fsf, F(ft), ftf), 0x3B: 'waitq',
            0x3C: 'mtir %s, %s%s' % (I(ft), F(fs), fsf), 0x3D: 'mfir.%s %s, %s' % (d, F(ft), I(fs)),
            0x3E: 'ilwr.%s %s, (%s)' % (d, I(ft), I(fs)), 0x3F: 'iswr.%s %s, (%s)' % (d, I(ft), I(fs)),
            0x40: 'rnext.%s %s, R' % (d, F(ft)), 0x41: 'rget.%s %s, R' % (d, F(ft)),
            0x42: 'rinit R, %s%s' % (F(fs), fsf), 0x43: 'rxor R, %s%s' % (F(fs), fsf),
            0x64: 'mfp.%s %s, P' % (d, F(ft)), 0x68: 'xtop %s' % I(ft), 0x69: 'xitop %s' % I(ft),
            0x6C: 'xgkick %s' % I(fs), 0x70: 'esadd P, %s' % F(fs), 0x71: 'ersadd P, %s' % F(fs),
            0x72: 'eleng P, %s' % F(fs), 0x73: 'erleng P, %s' % F(fs), 0x74: 'eatanxy P, %s' % F(fs),
            0x75: 'eatanxz P, %s' % F(fs), 0x76: 'esum P, %s' % F(fs), 0x78: 'esqrt P, %s%s' % (F(fs), fsf),
            0x79: 'ersqrt P, %s%s' % (F(fs), fsf), 0x7A: 'ercpr P, %s%s' % (F(fs), fsf), 0x7B: 'waitp',
            0x7C: 'esin P, %s%s' % (F(fs), fsf), 0x7D: 'eatan P, %s%s' % (F(fs), fsf),
            0x7E: 'eexp P, %s%s' % (F(fs), fsf)}
        return tab.get(k, '?lower %08x' % w)
    tab = {0x00: 'lq.%s %s, %d(%s)' % (d, F(ft), imm11, I(fs)), 0x01: 'sq.%s %s, %d(%s)' % (d, F(fs), imm11, I(ft)),
           0x04: 'ilw.%s %s, %d(%s)' % (d, I(ft), imm11, I(fs)), 0x05: 'isw.%s %s, %d(%s)' % (d, I(ft), imm11, I(fs)),
           0x08: 'iaddiu %s, %s, %d' % (I(ft), I(fs), imm15), 0x09: 'isubiu %s, %s, %d' % (I(ft), I(fs), imm15),
           0x10: 'fceq vi01, 0x%x' % (w & 0xFFFFFF), 0x11: 'fcset 0x%x' % (w & 0xFFFFFF),
           0x12: 'fcand vi01, 0x%x' % (w & 0xFFFFFF), 0x13: 'fcor vi01, 0x%x' % (w & 0xFFFFFF),
           0x14: 'fseq %s, 0x%x' % (I(ft), imm15), 0x15: 'fsset 0x%x' % imm15,
           0x16: 'fsand %s, 0x%x' % (I(ft), imm15), 0x17: 'fsor %s, 0x%x' % (I(ft), imm15),
           0x18: 'fmeq %s, %s' % (I(ft), I(fs)), 0x1A: 'fmand %s, %s' % (I(ft), I(fs)),
           0x1B: 'fmor %s, %s' % (I(ft), I(fs)), 0x1C: 'fcget %s' % I(ft),
           0x20: 'b %d' % (pc + 1 + imm11), 0x21: 'bal %s, %d' % (I(ft), pc + 1 + imm11),
           0x24: 'jr %s' % I(fs), 0x25: 'jalr %s, %s' % (I(ft), I(fs)),
           0x28: 'ibeq %s, %s, %d' % (I(ft), I(fs), pc + 1 + imm11), 0x29: 'ibne %s, %s, %d' % (I(ft), I(fs), pc + 1 + imm11),
           0x2C: 'ibltz %s, %d' % (I(fs), pc + 1 + imm11), 0x2D: 'ibgtz %s, %d' % (I(fs), pc + 1 + imm11),
           0x2E: 'iblez %s, %d' % (I(fs), pc + 1 + imm11), 0x2F: 'ibgez %s, %d' % (I(fs), pc + 1 + imm11)}
    return tab.get(op, '?lower %08x' % w)


def dis(code, base):
    for i in range(0, len(code), 8):
        lo, hi = struct.unpack_from('<II', code, i)
        pc = base + i // 8
        up = upper(hi)
        if hi >> 31 & 1:
            lw = 'loi %g' % struct.unpack('<f', struct.pack('<I', lo))[0]
        else:
            lw = lower(lo, pc)
        print('%04d  %-44s %s' % (pc, up, lw))


def walk(rd, a):
    while True:
        tag = struct.unpack('<Q', rd(a, 8))[0]
        qwc, tid, addr = tag & 0xFFFF, tag >> 28 & 7, tag >> 32 & 0x7FFFFFFF
        data = a + 16 if tid in (1, 6) else addr
        vifs = list(struct.unpack('<II', rd(a + 8, 8)))
        p, end = data, data + qwc * 16
        # VIF codes in the tag, then in the data
        def run_vif(v, p):
            cmd = v >> 24 & 0x7F
            if cmd == 0x4A:
                num = (v >> 16 & 0xFF) or 256
                base = v & 0xFFFF
                print('; MPG %d instructions at VU %d' % (num, base))
                dis(rd(p, num * 8), base)
                return p + num * 8
            return p
        for v in vifs:
            p = run_vif(v, p)
        while p < end:
            v = struct.unpack('<I', rd(p, 4))[0]
            p += 4
            p = run_vif(v, p)
        if tid in (6, 7, 0):   # ret / end / refe
            return
        a = a + 16 + qwc * 16 if tid == 1 else a + 16


if __name__ == '__main__':
    rd = load()
    if sys.argv[1] == '--mpg':
        a, n = int(sys.argv[2], 16), int(sys.argv[3])
        dis(rd(a, n * 8), 0)
    else:
        walk(rd, int(sys.argv[1], 16))
