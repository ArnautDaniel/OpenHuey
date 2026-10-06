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

add_executable(forth-tool src/forth/repl.c)
set_target_properties(forth-tool PROPERTIES OUTPUT_NAME forth)
target_link_libraries(forth-tool PRIVATE forth)
target_compile_definitions(forth-tool PRIVATE "HG2_SCRIPT_DIR=\"${SCRIPT_DIR}\"")

enable_testing()
file(GLOB FORTH_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.fs")
foreach(t ${FORTH_TESTS})
    get_filename_component(name "${t}" NAME_WE)
    add_test(NAME ${name} COMMAND forth-tool "${CMAKE_CURRENT_SOURCE_DIR}/tests/tester.fs" "${t}")
endforeach()
