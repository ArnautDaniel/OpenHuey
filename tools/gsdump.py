#!/usr/bin/env python3
# Decode the GS register stream (A+D packets) of an m2c draft: tools/decomp.py FUNC > f.c; tools/gsdump.py f.c
import re, sys
L = open(sys.argv[1]).read().split('\n')
regs = {0x00:'PRIM',0x01:'RGBAQ',0x02:'ST',0x03:'UV',0x05:'XYZ2',0x06:'TEX0',0x08:'CLAMP',0x14:'TEX1',0x16:'TEX2',0x18:'XYOFF',0x3F:'TEXFLUSH',0x40:'SCISSOR',0x42:'ALPHA',0x47:'TEST',0x4C:'FRAME',0x4E:'ZBUF',0x0D:'XYZ3'}
env = {}
def ev(e):
    e = re.sub(r'\((?:s64|u64|s32|u32|s128|f32)\)', '', e)
    e = re.sub(r'(0x[0-9A-Fa-f]+|\d+)U', r'\1', e)
    try: return eval(e, {}, env)
    except Exception: return None
def dec(reg, v):
    if v is None: return '?'
    if reg == 'XYZ2' or reg == 'XYZ3': return f'x={((v&0xFFFF)/16):.2f} y={(((v>>16)&0xFFFF)/16):.2f}'
    if reg == 'UV': return f'u={(v&0x3FFF)/16:.2f} v={((v>>16)&0x3FFF)/16:.2f}'
    if reg == 'XYOFF': return f'ox={(v&0xFFFF)/16:.1f} oy={((v>>32)&0xFFFF)/16:.1f}'
    if reg == 'SCISSOR': return f'x {v&0x7FF}..{(v>>16)&0x7FF} y {(v>>32)&0x7FF}..{(v>>48)&0x7FF}'
    if reg == 'FRAME': return f'fbp {v&0x1FF:#x} fbw {(v>>16)&0x3F} psm {(v>>24)&0x3F:#x} mask {v>>32:#x}'
    if reg == 'TEX0': return f'tbp {v&0x3FFF:#x}(page {(v&0x3FFF)//32:#x}) tbw {(v>>14)&0x3F} psm {(v>>20)&0x3F:#x} tw {1<<((v>>26)&0xF)} th {1<<((v>>30)&0xF)} tcc {(v>>34)&1} tfx {(v>>35)&3}'
    if reg == 'ALPHA':
        n = ['Cs','Cd','0','?']; c = ['As','Ad','FIX','?']
        return f'({n[v&3]}-{n[(v>>2)&3]})*{c[(v>>4)&3]}+{n[(v>>6)&3]} fix {(v>>32)&0xFF:#x}'
    if reg == 'CLAMP': return f'{v:#x}'
    return f'{v:#x}'
pend = {}
for l in L:
    s = l.strip()
    m = re.match(r'M2C_FIELD\((\w+), \w+ \*, (0x[0-9A-F]+|\d+)\) = (.*);$', s)
    if m:
        b, o, e = m.group(1), int(m.group(2), 0), m.group(3)
        v = ev(e)
        if o % 16 == 8 and v in regs:
            r = regs[v]; val = pend.get((b, o - 8))
            print(f'{b}+{o-8:#06x} {r:8} {dec(r, val[1]) if val else "?"}   {"" if val and val[1] is not None else (val[0] if val else "")}')
        else:
            pend[(b, o)] = (e, v)
        continue
    m = re.match(r'(\w+) = (.*);$', s)
    if m and not s.startswith('M2C'):
        v = ev(m.group(2))
        if v is not None: env[m.group(1)] = v
        else: env.pop(m.group(1), None)
        continue
    if re.search(r'^(if|else|do|\} while|\} else|loop|goto)', s): print('  ##', s)
