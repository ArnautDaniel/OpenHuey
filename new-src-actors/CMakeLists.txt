cmake_minimum_required(VERSION 3.20)
project(hga VERSION 0.0.1 LANGUAGES C)   # the version: shown with the debug mode (F2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)   # gnu11: _Noreturn, __attribute__, M_PI
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
add_compile_options(-Wall -Wextra -Wno-unused-parameter)

# A release (-DHGA_RELEASE=ON) knows no paths of this machine: the scripts are found beside the
# executable (scripts/), the game data is the player's own disc, chosen at start-up.
option(HGA_RELEASE "build for distribution (no built-in paths; Windows: no console)" OFF)
get_filename_component(SCRIPT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts" ABSOLUTE)
get_filename_component(DEFAULT_DATA "${CMAKE_CURRENT_SOURCE_DIR}/../../Haunting Ground (USA)/data" ABSOLUTE)
set(DEFAULT_EXE "${CMAKE_CURRENT_SOURCE_DIR}/../baserom/SLUS_210.75")
if(HGA_RELEASE)
    set(SCRIPT_DIR "")
    set(DEFAULT_DATA "")
    set(DEFAULT_EXE "")
endif()

# ---- the Forth system ----
add_library(forth STATIC src/forth/forth.c)
target_include_directories(forth PUBLIC src/forth)
target_link_libraries(forth PUBLIC m)

# the game state the scripts share (no SDL: the forth tool and the tests have it too)
add_library(gamestate STATIC src/game/progress.c)
target_link_libraries(gamestate PUBLIC forth)

# the actor kernel (no SDL either)
add_library(hactor STATIC src/hactor/hactor.c)
target_link_libraries(hactor PUBLIC forth)

add_executable(forth-tool src/forth/repl.c)
set_target_properties(forth-tool PROPERTIES OUTPUT_NAME forth)
target_link_libraries(forth-tool PRIVATE forth gamestate hactor)
target_compile_definitions(forth-tool PRIVATE "HG2_SCRIPT_DIR=\"${SCRIPT_DIR}\"")

enable_testing()
file(GLOB FORTH_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.fs")
foreach(t ${FORTH_TESTS})
    get_filename_component(name "${t}" NAME_WE)
    add_test(NAME ${name} COMMAND forth-tool "${t}")
endforeach()

# ---- the game ----
find_package(SDL3 REQUIRED CONFIG)
add_executable(hga
    src/main.c
    src/core/files.c
    src/platform/disc.c src/platform/library.c src/platform/gl.c src/platform/platform.c src/platform/sound.c src/platform/movie.c src/platform/music.c src/platform/seq.c src/platform/spu.c src/platform/snddrv.c
    src/render/render.c src/render/post.c
    src/data/pac.c src/data/tex.c src/data/roommesh.c src/data/model.c src/data/navmesh.c src/data/exe.c src/data/soundbank.c
    src/game/room.c src/game/console.c src/game/actor.c src/game/doglegs.c src/game/cloth.c src/game/world.c
    src/game/camdirector.c src/game/areas.c src/game/exits.c src/game/messages.c src/game/cutscene.c src/game/placed.c src/game/sprites.c src/game/particles.c src/game/things.c src/game/obstacles.c src/game/swimpath.c src/game/rumble.c
    src/forth/bind_engine.c)
target_link_libraries(hga PRIVATE forth gamestate hactor SDL3::SDL3)   # (GL through SDL_GL_GetProcAddress: platform/gl.c)
target_compile_definitions(hga PRIVATE "HGA_VERSION=\"${PROJECT_VERSION}\"" "HG2_SCRIPT_DIR=\"${SCRIPT_DIR}\"" "HG2_DEFAULT_DATA=\"${DEFAULT_DATA}\""
    "HG2_DEFAULT_EXE=\"${DEFAULT_EXE}\"")
if(HGA_RELEASE)
    if(WIN32)
        set_target_properties(hga PROPERTIES WIN32_EXECUTABLE ON)   # (SDL3 gives the WinMain)
    else()
        set_target_properties(hga PROPERTIES BUILD_RPATH "$ORIGIN/lib" INSTALL_RPATH "$ORIGIN/lib")
    endif()
endif()

# the subsystems' tests: headless in the game (they need the game's data), tests/<subsystem>/test_*.fs
file(GLOB GAME_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*/test_*.fs")
foreach(t ${GAME_TESTS})
    get_filename_component(name "${t}" NAME_WE)
    get_filename_component(dir "${t}" DIRECTORY)
    get_filename_component(sub "${dir}" NAME)
    add_test(NAME ${sub}_${name} COMMAND hga --test "${t}")
endforeach()
