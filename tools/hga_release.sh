#!/bin/sh
# hga's release builds, for 64-bit Linux and Windows, into dist/:
#   dist/hga-<version>-linux-x86_64.tar.gz    hga, scripts/, mod-examples/, lib/ (FFmpeg), licences
#   dist/hga-<version>-windows-x64.zip        hga.exe, SDL3.dll, FFmpeg's DLLs, scripts/, mod-examples/, licences
# Neither holds anything of the game's: the player picks their own disc image when hga starts.
#
# What they need is built once into $HGA_DEPS (default ~/.cache/openhuey-hga-deps - not in the
# repository: FFmpeg's configure can't take a path with spaces):
#   - SDL3, built static for Linux (it opens X11 / Wayland / the sound servers itself at run
#     time, so the player needs none of SDL); for Windows its MinGW package (SDL3.dll)
#   - FFmpeg's libavcodec and libavutil for the movies - only the MPEG-2 video decoder and its
#     parser (LGPL; nothing GPL or non-free, nothing found on the build machine)
#   - Zig's C compiler (zig cc), for Linux: it builds against an older glibc ($GLIBC), so the
#     game runs on older distributions than the build machine's (this system's headers for X11,
#     Wayland and the sound servers are still found, after Zig's libc)
# Needs: cmake, make, x86_64-w64-mingw32-gcc, curl, nasm, python3.
#   tools/hga_release.sh [linux|windows]   (both by default)
set -e
cd "$(dirname "$0")/.."
ROOT="$PWD"
SDL=3.4.18
FFMPEG=8.1.3
ZIG=0.15.2
GLIBC=2.31   # (the Linux build runs on this glibc and later: Ubuntu 20.04, Debian 11 on)
DEPS="${HGA_DEPS:-$HOME/.cache/openhuey-hga-deps}"
JOBS="${JOBS:-4}"
VERSION="${HGA_VERSION:-$(date +%Y%m%d)-$(git rev-parse --short HEAD 2>/dev/null || echo local)}"
WANT="${1:-all}"
mkdir -p "$DEPS" dist

fetch() {   # url dir: unpacked into $DEPS once
    [ -d "$DEPS/$2" ] || curl -L "$1" | tar xz -C "$DEPS"
}

ffmpeg_build() {   # name extra-configure-args...: avcodec / avutil, shared, into $DEPS/ffmpeg-<name>
    name="$1"
    shift
    [ -d "$DEPS/ffmpeg-$FFMPEG" ] || curl -L "https://ffmpeg.org/releases/ffmpeg-$FFMPEG.tar.xz" | tar xJ -C "$DEPS"
    [ -f "$DEPS/ffmpeg-$name/done" ] && return
    rm -rf "$DEPS/build-ffmpeg-$name"
    mkdir -p "$DEPS/build-ffmpeg-$name"
    (cd "$DEPS/build-ffmpeg-$name" &&
        "$DEPS/ffmpeg-$FFMPEG/configure" --prefix="$DEPS/ffmpeg-$name" \
            --enable-shared --disable-static --disable-autodetect --disable-programs --disable-doc \
            --disable-avdevice --disable-avformat --disable-avfilter --disable-swscale --disable-swresample \
            --disable-network --disable-everything --enable-decoder=mpeg2video --enable-parser=mpegvideo \
            --disable-debug "$@" &&
        sed -i 's/^#define HAVE_SYSCTL 1/#define HAVE_SYSCTL 0/' config.h &&   # (gone from glibc since 2.32)
        nice -n 19 make -j"$JOBS" && make install)
    cp "$DEPS/ffmpeg-$FFMPEG/COPYING.LGPLv2.1" "$DEPS/ffmpeg-$name/"
    touch "$DEPS/ffmpeg-$name/done"
}

licences() {   # into $1
    mkdir -p "$1/licences"
    cp LICENSE "$1/licences/OpenHuey-LICENSE.txt"
    cp "$DEPS/ffmpeg-$2/COPYING.LGPLv2.1" "$1/licences/FFmpeg-LGPLv2.1.txt"
    cp "$3" "$1/licences/SDL3-LICENSE.txt"
    cat > "$1/licences/FFmpeg-SOURCE.txt" <<EOT
The libavcodec / libavutil libraries here are FFmpeg $FFMPEG (https://ffmpeg.org), LGPL 2.1 or
later, unmodified, built by tools/hga_release.sh in the OpenHuey repository with only the
MPEG-2 video decoder and parser. Its source: https://ffmpeg.org/releases/ffmpeg-$FFMPEG.tar.xz
EOT
}

readme() {   # into $1 (the executable's name $2)
    cat > "$1/README.txt" <<EOT
hga - Haunting Ground, rebuilt (OpenHuey, https://github.com/ArnautDaniel/OpenHuey)

You need your own copy of Haunting Ground (USA) for the PlayStation 2: its disc image (.iso).
Start $2; the first time it asks for the disc image, and remembers it.
(The data can also be given as an argument: a .iso, a DATA.CVM, or the folder it was extracted to.)

Keys: WASD move, Space (hold) run, C act, Z shove / kick, V throw, Q (hold) crouch, E flee,
I J K L / F Hewie's commands, Return pause, Tab menu, \` the Forth console. A gamepad works too.
EOT
}

zig_cc() {   # $DEPS/zigcc: zig cc for the old glibc, as one command (cmake and configure take it as CC)
    if [ ! -x "$DEPS/zig-$ZIG/zig" ]; then
        curl -L "https://ziglang.org/download/$ZIG/zig-x86_64-linux-$ZIG.tar.xz" | tar xJ -C "$DEPS"
        mv "$DEPS/zig-x86_64-linux-$ZIG" "$DEPS/zig-$ZIG"
    fi
    cat > "$DEPS/zigcc" <<EOT
#!/bin/sh
exec "$DEPS/zig-$ZIG/zig" cc -target x86_64-linux-gnu.$GLIBC -idirafter /usr/include "\$@"
EOT
    chmod +x "$DEPS/zigcc"
}

linux() {
    zig_cc
    fetch "https://github.com/libsdl-org/SDL/releases/download/release-$SDL/SDL3-$SDL.tar.gz" "SDL3-$SDL"
    if [ ! -f "$DEPS/sdl3-linux/done" ]; then
        rm -rf "$DEPS/build-sdl3-linux"
        CC="$DEPS/zigcc" cmake -S "$DEPS/SDL3-$SDL" -B "$DEPS/build-sdl3-linux" -DCMAKE_BUILD_TYPE=Release \
            -DSDL_SHARED=OFF -DSDL_STATIC=ON -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF \
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_INSTALL_PREFIX="$DEPS/sdl3-linux"
        nice -n 19 cmake --build "$DEPS/build-sdl3-linux" -j"$JOBS"
        cmake --install "$DEPS/build-sdl3-linux"
        touch "$DEPS/sdl3-linux/done"
    fi
    ffmpeg_build linux --enable-pic --cc="$DEPS/zigcc"
    B="$ROOT/build/hga-release-linux"
    rm -rf "$B"
    CC="$DEPS/zigcc" cmake -S new-src-actors -B "$B" -DCMAKE_BUILD_TYPE=Release -DHGA_RELEASE=ON -DCMAKE_LINK_DEPENDS_USE_LINKER=OFF \
        -DCMAKE_PREFIX_PATH="$DEPS/sdl3-linux" -DSDL3_DIR="$(dirname "$(find "$DEPS/sdl3-linux" -name SDL3Config.cmake | head -1)")"
    cmake --build "$B" --target hga -j"$JOBS"
    P="dist/hga-$VERSION-linux-x86_64"
    rm -rf "$P"
    mkdir -p "$P/lib"
    cp "$B/hga" "$P/"
    strip "$P/hga"
    cp -r new-src-actors/scripts "$P/"
    cp -r new-src-actors/examples/mods "$P/mod-examples"   # (off: a mod is on once copied into mods/)
    cp -L "$DEPS/ffmpeg-linux/lib/libavcodec.so.62" "$DEPS/ffmpeg-linux/lib/libavutil.so.60" "$P/lib/"
    strip "$P/lib/"*.so.*
    licences "$P" linux "$DEPS/SDL3-$SDL/LICENSE.txt"
    readme "$P" "./hga"
    tar czf "$P.tar.gz" -C dist "$(basename "$P")"
    echo "made $P.tar.gz"
}

windows() {
    [ -d tools/win/SDL3-$SDL ] || { mkdir -p tools/win; curl -L "https://github.com/libsdl-org/SDL/releases/download/release-$SDL/SDL3-devel-$SDL-mingw.tar.gz" | tar xz -C tools/win; }
    WSDL="$ROOT/tools/win/SDL3-$SDL/x86_64-w64-mingw32"
    ffmpeg_build windows --target-os=mingw32 --arch=x86_64 --cross-prefix=x86_64-w64-mingw32- \
        --disable-pthreads --enable-w32threads --extra-ldflags=-static-libgcc
    B="$ROOT/build/hga-release-windows"
    cmake -S new-src-actors -B "$B" -DCMAKE_BUILD_TYPE=Release -DHGA_RELEASE=ON \
        -DCMAKE_TOOLCHAIN_FILE="$ROOT/new-src-actors/cmake/mingw-x86_64.cmake" \
        -DSDL3_DIR="$WSDL/lib/cmake/SDL3" -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static"
    cmake --build "$B" --target hga -j"$JOBS"
    P="dist/hga-$VERSION-windows-x64"
    rm -rf "$P"
    mkdir -p "$P"
    cp "$B/hga.exe" "$P/"
    x86_64-w64-mingw32-strip "$P/hga.exe"
    cp "$WSDL/bin/SDL3.dll" "$P/"
    cp "$DEPS/ffmpeg-windows/bin/avcodec-62.dll" "$DEPS/ffmpeg-windows/bin/avutil-60.dll" "$P/"
    cp -r new-src-actors/scripts "$P/"
    cp -r new-src-actors/examples/mods "$P/mod-examples"   # (off: a mod is on once copied into mods/)
    licences "$P" windows "$ROOT/tools/win/SDL3-$SDL/LICENSE.txt"
    readme "$P" "hga.exe"
    rm -f "$P.zip"
    (cd dist && python3 -c 'import shutil, sys; shutil.make_archive(sys.argv[1], "zip", ".", sys.argv[1])' "$(basename "$P")")
    echo "made $P.zip"
}

case "$WANT" in
    linux) linux ;;
    windows) windows ;;
    *) linux; windows ;;
esac
