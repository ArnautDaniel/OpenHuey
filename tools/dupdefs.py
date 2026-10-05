#!/usr/bin/env python3
"""List functions defined more than once across src/ and native/platform/ (a C definition that
duplicates another, e.g. a function decompiled twice). Static functions are per-file and are
ignored. Exit status 1 when any is found."""
import re, glob, sys, collections

DEF = re.compile(r'^(?!static\b|extern\b|typedef\b|#)[A-Za-z_][\w \t\*\(\)]*?\b(\w+)\s*\(([^;{}]*)\)\s*\{', re.M)
seen = collections.defaultdict(list)
for f in sorted(glob.glob('src/**/*.c', recursive=True) + glob.glob('native/platform/*.c')):
    text = open(f, errors='replace').read()
    # macro renames (debilitas2.c) define functions under other names: skip #define'd ones
    renamed = set(re.findall(r'^#define\s+(\w+)\s+\w+\s*$', text, re.M))
    for m in DEF.finditer(text):
        name = m.group(1)
        if name in ('if', 'while', 'for', 'switch', 'return', 'sizeof') or name in renamed:
            continue
        seen[name].append(f)
dups = {n: fs for n, fs in seen.items() if len(fs) > 1}
for n, fs in sorted(dups.items()):
    print(n, ' '.join(fs))
sys.exit(1 if dups else 0)
