#!/bin/sh
# Build the PC version and play into the first room: the boot logos, title and opening movie are
# skipped (HG_FASTBOOT, HG_ROOM=2A - what New Game starts), then room 0x2A runs with its event
# script, Fiona drawn and animated with OpenGL.
# Not yet: stalkers / event characters, music, some effects (logged once each as skip / glr).
set -e
cd "$(dirname "$0")/.."
cmake --build build/native/cmake -j4 >/dev/null
HG_FASTBOOT=1 HG_ROOM=2A HG_AUTOCROSS=1 HG_NOPARTNER=1 build/native/cmake/hg "$@" 2>&1 |
    grep -v "crifs\|iop:\|^event:\|PutDispEnv"
