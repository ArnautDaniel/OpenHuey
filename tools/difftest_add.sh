#!/bin/sh
# Append C (stdin) to FILE, difftest FUNC (a0 = an aligned object, plus the given options) and
# list it in tools/difftest_list.txt if it passes.
#   tools/difftest_add.sh src/game/x.c func_XXXXXXXX [difftest options] <<'EOF' ... EOF
cd "$(dirname "$0")/.."
f=$1; fn=$2; shift 2
{ echo; cat; } >> "$f"
.venv/bin/python tools/drop_leaf_dups.py >/dev/null 2>&1
r=$(.venv/bin/python tools/difftest.py "$f" "$fn" --pre=a0=0x0B000000..0x0B000000 "$@" 2>&1)
echo "$r" | grep -E "^(PASS|FAIL|ERROR)|call #|memory|error" | head -6
echo "$r" | grep -q "^PASS" && echo "$f $fn --pre=a0=0x0B000000..0x0B000000 $*" >> tools/difftest_list.txt
