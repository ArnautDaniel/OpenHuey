#!/bin/sh
# draft.sh FUNC...: m2c draft + note of calls passing $a4-$a7 / float args
for f in "$@"; do
  echo "// ===== $f"
  .venv/bin/python tools/decomp.py $f 2>&1 | grep -v "^$"
  sed -n "/glabel $f\$/,/endlabel/p" asm/game.s | sed -E 's#/\* [0-9A-F]+ ([0-9A-F]+) [0-9A-F]+ \*/ ##' | \
    awk '/\$a[4-7]|\$f1[2-9]/ && !/sq|lq|\(\$sp\)/ {buf=buf"   ;"$0} /jal|jalr/ {if (buf!="") print "//  ARGS:" buf " ->" $0; buf=""}'
done
