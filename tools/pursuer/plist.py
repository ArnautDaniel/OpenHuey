import re,glob,collections
asm=open('asm/game.s').read()
data=''.join(open(f).read() for f in glob.glob('asm/data/*.s'))
blocks=dict(re.findall(r'dlabel (\w+)\n(.*?)enddlabel',data,re.S))
slot={}
for i,x in enumerate(re.findall(r'\.word (\w+)',blocks['D_0046F6B0'])): slot.setdefault(x,hex(i*4))
calls=collections.Counter(re.findall(r'jal\s+(func_\w+)',asm))
rows=[]
import glob as G
defined=set()
for f in G.glob('src/**/*.c',recursive=True):
    defined|=set(re.findall(r'^(?!static\b|typedef\b|extern\b)[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{',open(f).read(),re.M))
for n,b in re.findall(r'glabel (\w+)\n(.*?)endlabel',asm,re.S):
    m=re.search(r'/\* [0-9A-F]+ ([0-9A-F]+) ',b)
    if not m: continue
    a=int(m.group(1),16)
    if 0x278490<=a<0x29FF10 or 0x211C80<=a<0x219530 or a in (0x1710D0,0x172810) or 0x179600<=a<0x179A00:
        if n in defined: continue
        k=len(re.findall(r'^\s*/\*',b,re.M))
        out=sorted(set(re.findall(r'jal\s+(\w+)',b)))
        rows.append((k,n,slot.get(n,''),calls[n],len(out)))
rows.sort()
for r in rows: print(*r)
