#!/bin/sh
# hg's release build for Windows, into dist/hg-<date>-<commit>-windows-x86.zip: hg.exe built with
# the game's data left out (HG_STRIP_DATA: copied from the player's own SLUS_210.75 at start-up),
# SDL3.dll, FFmpeg's DLLs for the movies (tools/win_deps.sh), the licences and a README. Nothing
# of the game's is in it: the player picks their own disc image when hg starts.
# Needs: tools/win_deps.sh run once, the split (configure.py --split), i686-w64-mingw32-gcc, python3.
set -e
cd "$(dirname "$0")/.."
ROOT="$PWD"
SDL=3.4.18
FFMPEG=8.1.3
JOBS="${JOBS:-4}"
VERSION="${HG_VERSION:-$(date +%Y%m%d)-$(git rev-parse --short HEAD 2>/dev/null || echo local)}"
B="$ROOT/build/native/win-release"

[ -d tools/win/SDL3-$SDL ] && [ -f tools/win/ffmpeg-$FFMPEG-i686/bin/avcodec-62.dll ] || tools/win_deps.sh
cmake -S native -B "$B" -G Ninja -DCMAKE_TOOLCHAIN_FILE="$ROOT/native/cmake/mingw-i686.cmake" \
    -DHG_STRIP_DATA=ON -DCMAKE_BUILD_TYPE=Release
nice -n 10 cmake --build "$B" -j"$JOBS"

P="dist/hg-$VERSION-windows-x86"
rm -rf "$P"
mkdir -p "$P/licences"
cp "$B/hg.exe" "$P/"
i686-w64-mingw32-strip "$P/hg.exe"
cp "tools/win/SDL3-$SDL/i686-w64-mingw32/bin/SDL3.dll" "tools/win/ffmpeg-$FFMPEG-i686/bin/avcodec-62.dll" \
    "tools/win/ffmpeg-$FFMPEG-i686/bin/avutil-60.dll" "$P/"
cp LICENSE "$P/licences/OpenHuey-LICENSE.txt"
cp "tools/win/SDL3-$SDL/LICENSE.txt" "$P/licences/SDL3-LICENSE.txt"
cp "tools/win/ffmpeg-$FFMPEG-i686/bin/COPYING.LGPLv2.1" "$P/licences/FFmpeg-LGPLv2.1.txt"
cat > "$P/licences/FFmpeg-SOURCE.txt" <<EOT
The libavcodec / libavutil DLLs here are FFmpeg $FFMPEG (https://ffmpeg.org), LGPL 2.1 or later,
unmodified, built by tools/win_deps.sh in the OpenHuey repository with only the MPEG-2 video
decoder and parser. Its source: https://ffmpeg.org/releases/ffmpeg-$FFMPEG.tar.xz
EOT
cat > "$P/README.txt" <<'EOT'
hg - Haunting Ground, decompiled and running natively (OpenHuey, https://github.com/ArnautDaniel/OpenHuey)

You need your own copy of Haunting Ground (USA) for the PlayStation 2: its disc image (.iso).
Start hg.exe; the first time it asks for the disc image, and remembers it. hg reads the game's
program (SLUS_210.75) and data from it - nothing of the game's is included here.

Keys: WASD move, Space (hold) run, C act, Z shove / kick, V items, E flee, I J K L / F Hewie's
commands, Return pause / skip, arrows the menus. F5 quicksaves, F9 loads it. A gamepad works too.
EOT
rm -f "$P.zip"
(cd dist && python3 -c 'import shutil, sys; shutil.make_archive(sys.argv[1], "zip", ".", sys.argv[1])' "$(basename "$P")")
echo "made $P.zip"
