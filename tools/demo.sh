#!/bin/sh
# Build the PC version and play into the first room: the title menu is passed automatically
# (New Game), then room 0x2A is drawn with OpenGL. Characters aren't updated or drawn yet
# (HG_NOCHARS) and the opening's "world stopped" flag is cleared each frame (HG_FREEPLAY).
set -e
cd "$(dirname "$0")/.."
cmake --build build/native/cmake -j4 >/dev/null
HG_FREEPLAY=1 HG_NOCHARS=1 HG_AUTOCROSS=1 HG_NOPARTNER=1 build/native/cmake/hg "$@" 2>&1 |
    grep -v "crifs\|iop:\|^event:\|PutDispEnv"
