# addlist.py OPTSFILE SRC...: list every function defined in SRC not yet in tools/difftest_list.txt
import re,sys
opts={}
for l in open(sys.argv[1]):
    p=l.split(None,2)
    if len(p)>=2: opts[(p[0],p[1])]=p[2].strip() if len(p)>2 else ''
L=open('tools/difftest_list.txt').read().rstrip('\n').split('\n')
have={tuple(l.split()[:2]) for l in L if l.strip()}
add=[]
for src in sys.argv[2:]:
    for fn in re.findall(r'^(?!extern|static)[A-Za-z][^\n;]*?\b(func_[0-9A-F]{8})\([^;{]*\)\s*\{',open(src).read(),re.M):
        if (src,fn) not in have:
            add.append(f"{src} {fn} {opts.get((src,fn),'')}".rstrip()); have.add((src,fn))
open('tools/difftest_list.txt','w').write('\n'.join(L+add)+'\n')
print(len(add),'added')
