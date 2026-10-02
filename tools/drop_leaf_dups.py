#!/usr/bin/env python3
"""Remove from src/leaf/*.c the functions now defined in src/game/*.c (and their entries in
tools/difftest_list.txt). Leaf functions were decompiled in bulk early on; when one is rewritten
as part of its class, the class file is the one to keep. Run before configure.py / ninja."""
import glob
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEF = r"^(?!extern|static)[A-Za-z][\w \*]*?\b(func_[0-9A-F]{8})\([^;]*?\)\s*\{"

defs = set()
for f in glob.glob(str(ROOT / "src/game/*.c")):
    defs |= set(re.findall(DEF, Path(f).read_text(), re.M))
removed = []
for f in sorted(glob.glob(str(ROOT / "src/leaf/*.c"))):
    p = Path(f)
    s = o = p.read_text()
    for m in list(re.finditer(DEF + r".*?^\}\n\n?", s, re.M | re.S)):
        if m.group(1) in defs:
            s = s.replace(m.group(0), "", 1)
            removed.append((str(p.relative_to(ROOT)), m.group(1)))
    if s != o:
        p.write_text(s)
lst = ROOT / "tools/difftest_list.txt"
rm = set(removed)
lines = [l for l in lst.read_text().split("\n")
         if not (len(l.split()) > 1 and (l.split()[0], l.split()[1]) in rm)]
lst.write_text("\n".join(lines))
for f, n in removed:
    print(f"removed {n} from {f}")
