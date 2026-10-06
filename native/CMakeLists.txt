# Native (x86, 32-bit) build of the decompiled game.
#
#   cmake -S native -B build/native/cmake && cmake --build build/native/cmake -j4
#   build/native/cmake/hg <extracted DATA.CVM folder>
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
add_compile_options(-m32 -fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks)
add_link_options(-m32 -no-pie -rdynamic)

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
add_custom_command(
    OUTPUT ${DATA_S} ${GEN}/stubs.c ${GEN}/thunks.S
    COMMAND ${PY} ${ROOT}/tools/native_gen.py -o ${GEN}
    DEPENDS ${ROOT}/tools/native_gen.py ${PS2_DATA} ${ROOT}/asm/game.s ${ROOT}/asm/sinit.s
    COMMENT "Generating native data and stubs"
    VERBATIM)

file(GLOB GAME_C CONFIGURE_DEPENDS ${ROOT}/src/game/*.c ${ROOT}/src/game/rooms/*.c ${ROOT}/src/sdk/*.c)
file(GLOB PLATFORM_C CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/platform/*.c)

add_executable(hg ${DATA_S} ${GEN}/stubs.c ${GEN}/thunks.S ${GAME_C} ${PLATFORM_C})
target_include_directories(hg PRIVATE ${ROOT}/include ${ROOT}/src ${CMAKE_CURRENT_SOURCE_DIR}/platform)
# the decompiled code type-puns freely and relies on wrapping arithmetic like the original
target_compile_options(hg PRIVATE $<$<COMPILE_LANGUAGE:C>:-g -O2 -fno-strict-aliasing -fwrapv -Wno-int-conversion>)
target_compile_definitions(hg PRIVATE HG_NATIVE)
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
