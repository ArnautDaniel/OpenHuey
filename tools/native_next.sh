#!/bin/sh
# Build the PC version, run it into a room (HG_ROOM, default 2A) without the partner, and show
# the first function it needs that isn't decompiled yet: its callers and its asm.
#   tools/native_next.sh               FRAMES=n (default 1500), ROOM=hex, DUMP=dir, ASM=lines
cd "$(dirname "$0")/.."
cmake --build build/native/cmake -j4 2>&1 | grep -E "error|warning: [^m]" | head
# ROOM=menu: through the title menu (New Game, Cross pressed every 2 s) instead of HG_ROOM
if [ "${ROOM:-2A}" = menu ]; then export HG_AUTOCROSS=1; else export HG_ROOM=${ROOM:-2A}; fi
out=$({ HG_NOPARTNER=1 HG_HEADLESS=1 HG_MAXFRAMES=${FRAMES:-1500} HG_DUMP=${DUMP:-} \
      timeout 300 build/native/cmake/hg 2>&1; echo "exit status $?"; } | grep -v "crifs\|iop:")
echo "$out" | grep "^event:" | head -20
echo "$out" | grep -A5 "undecompiled\|Segmentation\|runaway\|vu1:" | head -8
# a crash (the shell reports it, not the game): where, via gdb
if echo "$out" | grep -q "^exit status 1[34][0-9]"; then
    echo "crashed: $(echo "$out" | tail -1)"
    HG_NOPARTNER=1 HG_HEADLESS=1 HG_MAXFRAMES=${FRAMES:-1500} timeout 300 gdb -q -batch -ex run -ex bt \
        build/native/cmake/hg 2>&1 | sed -n '/^Program received/,$p' | head -12
fi
fn=$(echo "$out" | sed -n 's/^undecompiled: \([A-Za-z0-9_]*\) .*/\1/p' | head -1)
if [ -n "$fn" ]; then
    sed -n "/glabel $fn\$/,/endlabel/p" asm/game.s | sed -E 's#/\* [0-9A-F]+ ([0-9A-F]+) [0-9A-F]+ \*/#\1#' |
        grep -v "^\s*$" | head -${ASM:-400}
else
    echo "$out" | tail -5
fi
