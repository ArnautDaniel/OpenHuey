cmake_minimum_required(VERSION 3.20)
project(hg2 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)   # gnu11: _Noreturn, __attribute__, M_PI
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
add_compile_options(-Wall -Wextra -Wno-unused-parameter)

get_filename_component(SCRIPT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts" ABSOLUTE)
get_filename_component(DEFAULT_DATA "${CMAKE_CURRENT_SOURCE_DIR}/../../Haunting Ground (USA)/data" ABSOLUTE)

# ---- the Forth system ----
add_library(forth STATIC src/forth/forth.c)
target_include_directories(forth PUBLIC src/forth)
target_link_libraries(forth PUBLIC m)

# the game state the scripts share (no SDL: the forth tool and the tests have it too)
add_library(gamestate STATIC src/game/progress.c)
target_link_libraries(gamestate PUBLIC forth)

add_executable(forth-tool src/forth/repl.c)
set_target_properties(forth-tool PROPERTIES OUTPUT_NAME forth)
target_link_libraries(forth-tool PRIVATE forth gamestate)
target_compile_definitions(forth-tool PRIVATE "HG2_SCRIPT_DIR=\"${SCRIPT_DIR}\"")

enable_testing()
file(GLOB FORTH_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.fs")
foreach(t ${FORTH_TESTS})
    get_filename_component(name "${t}" NAME_WE)
    add_test(NAME ${name} COMMAND forth-tool "${t}")
endforeach()

# ---- the game ----
find_package(SDL3 REQUIRED CONFIG)
find_package(OpenGL REQUIRED)
add_executable(hg2
    src/main.c
    src/core/files.c
    src/platform/gl.c src/platform/platform.c src/platform/sound.c src/platform/movie.c src/platform/music.c src/platform/seq.c src/platform/spu.c src/platform/snddrv.c
    src/render/render.c src/render/post.c
    src/data/pac.c src/data/tex.c src/data/roommesh.c src/data/model.c src/data/navmesh.c src/data/exe.c src/data/soundbank.c
    src/game/room.c src/game/console.c src/game/actor.c src/game/doglegs.c src/game/world.c
    src/game/camdirector.c src/game/areas.c src/game/exits.c src/game/messages.c src/game/cutscene.c src/game/placed.c
    src/forth/bind_engine.c)
target_link_libraries(hg2 PRIVATE forth gamestate SDL3::SDL3 OpenGL::GL ${CMAKE_DL_LIBS})
target_compile_definitions(hg2 PRIVATE "HG2_SCRIPT_DIR=\"${SCRIPT_DIR}\"" "HG2_DEFAULT_DATA=\"${DEFAULT_DATA}\""
    "HG2_DEFAULT_EXE=\"${CMAKE_CURRENT_SOURCE_DIR}/../baserom/SLUS_210.75\"")
