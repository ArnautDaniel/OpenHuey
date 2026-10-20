cmake_minimum_required(VERSION 3.20)
project(hga VERSION 0.1.14 LANGUAGES C)   # the version: shown with the debug mode (F2)

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
# the build's name, which the updater compares with the newest release's (src/platform/update.c):
# a release's is the version and its commit (tools/hga_release.sh: "0.1.1-c6b7113")
if(NOT HGA_BUILD)
    set(HGA_BUILD "${PROJECT_VERSION}")
endif()
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
# the game's data: its files (the disc, the library, the mods' layer), the formats and the
# library's conversions - the engine's and the checks' (tests/*_check.c)
add_library(hgadata STATIC
    src/core/files.c src/core/zip.c src/platform/disc.c src/platform/library.c
    src/data/pac.c src/data/roommesh.c src/data/gltfio.c src/data/roomgltf.c src/data/modelgltf.c src/data/roomtext.c src/data/tex.c src/data/texbank.c src/data/musicsrc.c
    src/data/flacenc.c src/data/msgtext.c src/data/model.c src/data/navmesh.c src/data/exe.c src/data/exetables.c
    src/data/soundbank.c src/data/vag.c src/data/samples.c src/data/sfd.c src/data/artpic.c
    src/third_party/stb.c src/third_party/dr_flac.c src/third_party/cgltf.c)
target_link_libraries(hgadata PUBLIC SDL3::SDL3 m)

add_executable(hga
    src/main.c
    src/platform/gl.c src/platform/platform.c src/platform/update.c src/platform/sound.c src/platform/movie.c src/platform/music.c src/platform/seq.c src/platform/spu.c src/platform/snddrv.c
    src/render/render.c src/render/post.c
    src/game/room.c src/game/console.c src/game/actor.c src/game/doglegs.c src/game/cloth.c src/game/world.c
    src/game/camdirector.c src/game/areas.c src/game/exits.c src/game/messages.c src/game/cutscene.c src/game/placed.c src/game/sprites.c src/game/mods.c src/game/particles.c src/game/things.c src/game/obstacles.c src/game/swimpath.c src/game/rumble.c
    src/forth/bind_engine.c)
target_link_libraries(hga PRIVATE hgadata forth gamestate hactor SDL3::SDL3)
set_target_properties(hga PROPERTIES OUTPUT_NAME openhuey)   # (the target keeps its old name: the tests' COMMAND hga)   # (GL through SDL_GL_GetProcAddress: platform/gl.c)
target_compile_definitions(hga PRIVATE "HGA_VERSION=\"${PROJECT_VERSION}\"" "HGA_BUILD=\"${HGA_BUILD}\"" "HG2_SCRIPT_DIR=\"${SCRIPT_DIR}\"" "HG2_DEFAULT_DATA=\"${DEFAULT_DATA}\""
    "HG2_DEFAULT_EXE=\"${DEFAULT_EXE}\"")
if(HGA_RELEASE)
    target_compile_definitions(hga PRIVATE HGA_UPDATER)   # (a release updates itself: --update, the title's UPDATE)
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

# the textures' conversion: every bank of the developer's data converted and read back, the same pixels
add_executable(texbank-check tests/texbank_check.c)
target_link_libraries(texbank-check PRIVATE hgadata)
add_test(NAME texbank_check COMMAND texbank-check "${DEFAULT_DATA}" "${CMAKE_CURRENT_BINARY_DIR}/texbank-check.scratch")
set_tests_properties(texbank_check PROPERTIES SKIP_RETURN_CODE 77)

# the sample mod, turned on: tests/mods/*.fs (not test_*.fs - the rest run without mods)
add_test(NAME mods_room_tally COMMAND hga --mod "${CMAKE_CURRENT_SOURCE_DIR}/examples/mods/room-tally"
    --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/mods/room-tally.fs")

# mods' manifests and files: the order, what turns a mod off, the files layer, a mod's textures
add_executable(mods-check tests/mods_check.c src/game/mods.c)
target_link_libraries(mods-check PRIVATE hgadata forth)
add_test(NAME mods_check COMMAND mods-check "${DEFAULT_DATA}" "${CMAKE_CURRENT_BINARY_DIR}/mods-check.scratch")
set_tests_properties(mods_check PROPERTIES SKIP_RETURN_CODE 77)
# ... and zipped (the folder inside the zip, as zipping a folder makes it)
add_test(NAME mods_zip_sample COMMAND ${CMAKE_COMMAND} -E tar cf "${CMAKE_CURRENT_BINARY_DIR}/room-tally.zip" --format=zip room-tally
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/examples/mods")
set_tests_properties(mods_zip_sample PROPERTIES FIXTURES_SETUP room_tally_zip)
add_test(NAME mods_room_tally_zipped COMMAND hga --mod "${CMAKE_CURRENT_BINARY_DIR}/room-tally.zip"
    --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/mods/room-tally.fs")
set_tests_properties(mods_room_tally_zipped PROPERTIES FIXTURES_REQUIRED room_tally_zip)

# the music's conversion: every ADX made a FLAC and both played, looped too - the same samples
add_executable(music-check tests/music_check.c)
target_link_libraries(music-check PRIVATE hgadata)
add_test(NAME music_check COMMAND music-check "${DEFAULT_DATA}")
set_tests_properties(music_check PROPERTIES SKIP_RETURN_CODE 77)

# the codecs: optimized in every build (the import's encoding, the checks, the music's decoding)
set_source_files_properties(src/data/flacenc.c src/data/musicsrc.c src/third_party/dr_flac.c src/third_party/stb.c
    PROPERTIES COMPILE_OPTIONS "-O2")

# the messages' conversion: every table written as text and read back - the same messages
add_executable(messages-check tests/messages_check.c)
target_link_libraries(messages-check PRIVATE hgadata)
add_test(NAME messages_check COMMAND messages-check "${DEFAULT_DATA}")
set_tests_properties(messages_check PROPERTIES SKIP_RETURN_CODE 77)
add_test(NAME mods_flower_scent COMMAND hga --mod "${CMAKE_CURRENT_SOURCE_DIR}/examples/mods/flower-scent"
    --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/mods/flower-scent.fs")

# the sound banks' conversion: every sample made a WAV and both played as the SPU plays them
add_executable(sound-check tests/sound_check.c)
target_link_libraries(sound-check PRIVATE hgadata)
add_test(NAME sound_check COMMAND sound-check "${DEFAULT_DATA}")
set_tests_properties(sound_check PROPERTIES SKIP_RETURN_CODE 77)

# the movies' conversion: every SFD split as the import splits it - the same video packets, the same sound
add_executable(movie-check tests/movie_check.c)
target_link_libraries(movie-check PRIVATE hgadata)
add_test(NAME movie_check COMMAND movie-check "${DEFAULT_DATA}")
set_tests_properties(movie_check PROPERTIES SKIP_RETURN_CODE 77)

# the rooms' conversion: each PAC less its converted sections - the rest the same bytes and sizes
add_executable(room-check tests/room_check.c)
target_link_libraries(room-check PRIVATE hgadata)
add_test(NAME room_check COMMAND room-check "${DEFAULT_DATA}")
set_tests_properties(room_check PROPERTIES SKIP_RETURN_CODE 77)

# the models' conversion: every .PCK as glTF (+ its motion bank) and read back - the same Model
add_executable(model-check tests/model_check.c)
target_link_libraries(model-check PRIVATE hgadata)
add_test(NAME model_check COMMAND model-check "${DEFAULT_DATA}")
set_tests_properties(model_check PROPERTIES SKIP_RETURN_CODE 77)
# the mods menu: a copy of the sample mods as the mods folder - driven, saved, then read again
add_test(NAME mods_menu_folder COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/examples/mods"
    "${CMAKE_CURRENT_BINARY_DIR}/mods-menu.scratch")
set_tests_properties(mods_menu_folder PROPERTIES FIXTURES_SETUP mods_menu_folder)
add_test(NAME mods_menu_clean COMMAND ${CMAKE_COMMAND} -E rm -f "${CMAKE_CURRENT_BINARY_DIR}/mods-menu.scratch/order.txt")
set_tests_properties(mods_menu_clean PROPERTIES FIXTURES_SETUP mods_menu_folder DEPENDS mods_menu_folder)
add_test(NAME mods_menu COMMAND hga --mods "${CMAKE_CURRENT_BINARY_DIR}/mods-menu.scratch"
    --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/mods/menu.fs")
set_tests_properties(mods_menu PROPERTIES FIXTURES_REQUIRED mods_menu_folder FIXTURES_SETUP mods_menu_saved)
add_test(NAME mods_menu_after COMMAND hga --mods "${CMAKE_CURRENT_BINARY_DIR}/mods-menu.scratch"
    --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/mods/menu-after.fs")
set_tests_properties(mods_menu_after PROPERTIES FIXTURES_REQUIRED "mods_menu_folder;mods_menu_saved")
# the title as a player meets it: --title brings it up headless too (not a test_*.fs: those run
# without it, straight into a new game)
add_test(NAME title_title COMMAND hga --title --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/title/title.fs")
# the updater: against releases made in the test (file://), and the title's UPDATE page
if(NOT WIN32)
    add_test(NAME update_run COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/tests/update/run.sh" $<TARGET_FILE:hga> "${HGA_BUILD}"
        "${CMAKE_CURRENT_BINARY_DIR}")
endif()
string(REPLACE " " "%20" NO_RELEASE_URL "file://${CMAKE_CURRENT_BINARY_DIR}/no-release/")
add_test(NAME update_title COMMAND hga --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/update/title.fs")
set_tests_properties(update_title PROPERTIES ENVIRONMENT
    "OPENHUEY_UPDATE_URL=${NO_RELEASE_URL};OPENHUEY_UPDATE_DIR=${CMAKE_CURRENT_BINARY_DIR}/update-title.scratch")
add_test(NAME mods_first_person COMMAND hga --mod "${CMAKE_CURRENT_SOURCE_DIR}/examples/mods/first-person"
    --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/mods/first-person.fs")
add_test(NAME mods_free_play_entry COMMAND hga --mod "${CMAKE_CURRENT_SOURCE_DIR}/examples/mods/free-play-entry"
    --test "${CMAKE_CURRENT_SOURCE_DIR}/tests/mods/free-play-entry.fs")
