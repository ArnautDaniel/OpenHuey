#!/bin/sh
# The Windows build's libraries, into tools/win (ignored by git):
#   - SDL3's MinGW development package (headers, the i686 import library and SDL3.dll);
#   - FFmpeg's libavcodec and libavutil for the movies, built from source for 32-bit Windows with
#     only what Sofdec needs (the MPEG-2 video decoder and parser). LGPL: no GPL or non-free parts,
#     nothing found on the build machine (--disable-autodetect). No assembly: FFmpeg's SSE code
#     assumes a 16-byte aligned stack, which 32-bit Windows doesn't give (MPEG-2 at 640 x 448 is
#     cheap in C). Needs make.
set -e
cd "$(dirname "$0")/.."
SDL=3.4.18
FFMPEG=8.1.3
mkdir -p tools/win
if [ ! -d tools/win/SDL3-$SDL ]; then
    curl -L "https://github.com/libsdl-org/SDL/releases/download/release-$SDL/SDL3-devel-$SDL-mingw.tar.gz" \
        | tar xz -C tools/win
fi
echo "SDL3 $SDL in tools/win/SDL3-$SDL/i686-w64-mingw32"

OUT="$PWD/tools/win/ffmpeg-$FFMPEG-i686"
if [ ! -f "$OUT/bin/avcodec-62.dll" ]; then
    if [ ! -d tools/win/ffmpeg-$FFMPEG ]; then
        curl -L "https://ffmpeg.org/releases/ffmpeg-$FFMPEG.tar.xz" | tar xJ -C tools/win
    fi
    cd tools/win/ffmpeg-$FFMPEG
    # (the prefix isn't used: the DLLs are copied from the build; a path with spaces breaks it)
    ./configure --prefix=/opt/ffmpeg --target-os=mingw32 --arch=x86 --cross-prefix=i686-w64-mingw32- \
        --enable-shared --disable-static --disable-autodetect --disable-programs --disable-doc \
        --disable-avdevice --disable-avformat --disable-avfilter --disable-swscale --disable-swresample \
        --disable-network --disable-everything --enable-decoder=mpeg2video --enable-parser=mpegvideo \
        --disable-pthreads --enable-w32threads --disable-debug --disable-asm \
        --extra-ldflags="-static-libgcc"
    nice -n 19 make -j"${JOBS:-4}"
    mkdir -p "$OUT/bin"
    cp libavcodec/avcodec-62.dll libavutil/avutil-60.dll COPYING.LGPLv2.1 "$OUT/bin/"
    cd ../../..
fi
echo "FFmpeg $FFMPEG (avcodec, avutil) in $OUT/bin"
