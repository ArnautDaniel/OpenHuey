# Native (x86, 32-bit) build of the decompiled game.
#
#   cmake -S native -B build/native/cmake && cmake --build build/native/cmake -j4
#   build/native/cmake/hg <extracted DATA.CVM folder>
#
# 32-bit Windows (MinGW-w64, cross-compiled): tools/win_deps.sh, then
#   cmake -S native -B build/native/win -DCMAKE_TOOLCHAIN_FILE=native/cmake/mingw-i686.cmake
#   cmake --build build/native/win -j8      (hg.exe and SDL3.dll in build/native/win)
#
# 32-bit for now: the decompiled C still relies on the PS2's 4-byte pointers (raw offsets into
# objects, pointer tables in the game data). Functions not written in C yet are weak stubs
# that stop with "undecompiled func_XXXXXXXX" (tools/native_gen.py).
cmake_minimum_required(VERSION 3.20)
project(haunting_ground_native C ASM)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(GEN ${CMAKE_CURRENT_BINARY_DIR}/gen)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(CMAKE_C_STANDARD 11)
# the game C type-puns through raw offsets and relies on wrapping arithmetic, like the PS2 build
add_compile_options(-fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks)
if(WIN32)
    # GCC's struct and bitfield layout, as on Linux (MinGW defaults to Microsoft's)
    add_compile_options(-mno-ms-bitfields)
    set(NATIVE_GEN_FLAGS --windows)
else()
    add_compile_options(-m32)
    add_link_options(-m32 -no-pie -rdynamic)
    set(NATIVE_GEN_FLAGS)
endif()

find_package(Python3 REQUIRED COMPONENTS Interpreter)
if(EXISTS ${ROOT}/.venv/bin/python)
    set(PY ${ROOT}/.venv/bin/python)
else()
    set(PY ${Python3_EXECUTABLE})
endif()

file(GLOB PS2_DATA ${ROOT}/asm/data/*.s)
# order matters: the static constructor table spans sinit_ctors + sinit_data
set(DATA_S
    ${GEN}/data/sinit_ctors.data.S
    ${GEN}/data/sinit_data.data.S
    ${GEN}/data/2AC800.data.S
    ${GEN}/data/34AD80.rodata.S
    ${GEN}/data/37B280.bss.S)
# HG_STRIP_DATA: the game's own data left out of hg, copied from the player's disc at startup
# (native/platform/slus.c) - for builds given to others. Default: in the build.
option(HG_STRIP_DATA "leave the game's data out of hg (read from SLUS_210.75 on the disc)" OFF)
# HG_RELEASE: a build for players - no debugging shown (the room in the corner: HG_SHOW_ROOM=1 shows it)
option(HG_RELEASE "a build for players: no debugging shown" OFF)
if(HG_STRIP_DATA)
    list(APPEND NATIVE_GEN_FLAGS --strip)
endif()
add_custom_command(
    OUTPUT ${DATA_S} ${GEN}/stubs.c ${GEN}/thunks.S ${GEN}/slus.c
    COMMAND ${PY} ${ROOT}/tools/native_gen.py -o ${GEN} ${NATIVE_GEN_FLAGS}
    DEPENDS ${ROOT}/tools/native_gen.py ${PS2_DATA} ${ROOT}/asm/game.s ${ROOT}/asm/sinit.s
            ${CMAKE_CURRENT_SOURCE_DIR}/data/group1.txt ${ROOT}/baserom/SLUS_210.75
    COMMENT "Generating native data and stubs"
    VERBATIM)

file(GLOB GAME_C CONFIGURE_DEPENDS ${ROOT}/src/game/*.c ${ROOT}/src/game/rooms/*.c ${ROOT}/src/sdk/*.c)
file(GLOB PLATFORM_C CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/platform/*.c)
# the compiler-generated part of the game's data as C (tools/data_c.py): the static constructor
# table, the vtables, the pointer-to-member constants
file(GLOB DATA_C CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/data/*.c)

add_executable(hg ${DATA_S} ${GEN}/stubs.c ${GEN}/thunks.S ${GEN}/slus.c ${GAME_C} ${PLATFORM_C} ${DATA_C})
target_include_directories(hg PRIVATE ${ROOT}/include ${ROOT}/src ${CMAKE_CURRENT_SOURCE_DIR}/platform)
# the decompiled code type-puns freely and relies on wrapping arithmetic like the original
target_compile_options(hg PRIVATE $<$<COMPILE_LANGUAGE:C>:-g -O2 -fno-strict-aliasing -fwrapv -Wno-int-conversion>)
target_compile_definitions(hg PRIVATE HG_NATIVE)
if(HG_RELEASE)
    target_compile_definitions(hg PRIVATE HG_RELEASE)
endif()
if(WIN32)
    # the data folder from the argument or $HG_DATA; else "data" next to hg.exe
    target_compile_definitions(hg PRIVATE "HG_DEFAULT_DATA=\"data\"")
    set(SDL3_WIN ${ROOT}/tools/win/SDL3-3.4.18/i686-w64-mingw32)
    if(NOT EXISTS ${SDL3_WIN})
        message(FATAL_ERROR "SDL3 for MinGW not found: run tools/win_deps.sh")
    endif()
    target_include_directories(hg PRIVATE ${SDL3_WIN}/include)
    target_link_libraries(hg PRIVATE ${SDL3_WIN}/lib/libSDL3.dll.a)
    add_custom_command(TARGET hg POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL3_WIN}/bin/SDL3.dll $<TARGET_FILE_DIR:hg>
        VERBATIM)
    # the movies' decoder (loaded at run time; LGPL, built by tools/win_deps.sh)
    set(FFMPEG_WIN ${ROOT}/tools/win/ffmpeg-8.1.3-i686/bin)
    if(EXISTS ${FFMPEG_WIN}/avcodec-62.dll)
        add_custom_command(TARGET hg POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FFMPEG_WIN}/avcodec-62.dll ${FFMPEG_WIN}/avutil-60.dll
                    $<TARGET_FILE_DIR:hg>
            VERBATIM)
    else()
        message(WARNING "FFmpeg for Windows not built (tools/win_deps.sh): hg.exe will skip the movies")
    endif()
    return()
endif()
# the default data folder: the extracted DATA.CVM next to the repository, as an absolute path so
# hg runs from any directory (override with an argument or $HG_DATA)
get_filename_component(HG_DEFAULT_DATA "${ROOT}/../Haunting Ground (USA)/data" ABSOLUTE)
target_compile_definitions(hg PRIVATE "HG_DEFAULT_DATA=\"${HG_DEFAULT_DATA}\"")
# SDL3 (32-bit): Gentoo's multilib puts it in /usr/lib (CMake would rewrite that to lib64)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)
find_library(SDL3_LIBRARY NAMES SDL3 PATHS /usr/lib /usr/lib32 /usr/lib/i386-linux-gnu NO_DEFAULT_PATH)
if(NOT SDL3_LIBRARY)
    find_library(SDL3_LIBRARY NAMES SDL3 REQUIRED)
endif()
target_link_libraries(hg PRIVATE ${SDL3_LIBRARY} m ${CMAKE_DL_LIBS})
