#!/bin/sh
# Difftest event command opcodes of func_002029B0 (or another command interpreter, $FUNC) one
# opcode at a time: tools/difftest_evcmd.sh 0x00 0x05 ...  (adds passing ones to the list)
cd "$(dirname "$0")/.."
f=${FILE:-src/game/event_cmd.c}; fn=${FUNC:-func_002029B0}
V=0x0046B3A0; O=0x0046BF20
for op in "$@"; do
    o="--pre=a0=0x0B000000..0x0B000000 --pre=a0+0=$V..$V --pre=a0+4=0x0B100000..0x0B100000 --pre=@0x0B100000=bytes:$(printf %02X $op)???????????????????????????????? --pre=@0x44E4D8=0x0B500000..0x0B500000 --pre=@0x0B500000=$O..$O --pre=@0x44F818=0x0B300000..0x0B300000 --pre=@0x44F820=0x0B400000..0x0B400000 --pre=@0x44E4B8=0x0B900000..0x0B900000 --pre=@0x0B900000=$O..$O --stub-ret=0 --stub-ret=1 --stub-ret=2 --stub-ret=0xFF --runs=30 $EXTRA"
    r=$(.venv/bin/python tools/difftest.py "$f" "$fn" $o 2>&1)
    echo "$op: $(echo "$r" | grep -E "^(PASS|FAIL|ERROR)|memory|error|call #" | head -3)"
    echo "$r" | grep -q "^PASS" && echo "$f $fn $o" >> tools/difftest_list.txt
done
