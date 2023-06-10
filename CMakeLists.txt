cmake_minimum_required(VERSION 3.9.0)
project(risc-vliw-2023 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)


# Functions
function(architecture arch_name)
    file(GLOB_RECURSE arch_sources src/arch/${arch_name}/*.cpp)
    set(${arch_name}_sources ${arch_sources} PARENT_SCOPE)
    add_executable(emu_${arch_name} src/main/emu.cpp ${arch_sources} ${common_sources})
endfunction(architecture)


# Sources
file(GLOB_RECURSE common_sources src/common/*)
include_directories(src/inc)


# CPU architectures
architecture(ez8)

# Testing
include(CTest)
enable_testing()


file(GLOB test_dir "test")
message("test_dir: " ${test_dir})
file(GLOB test_sources RELATIVE ${test_dir}/.. "test/*.cpp")
message("test_sources: " ${test_sources})


foreach(test ${test_sources})
    message("reading test file: " ${test})
    file(STRINGS ${test} shoulds REGEX "^[ ]*SHOULD\\(\".*\"\\)")

    string(REGEX REPLACE ".cpp$" "" test_entry ${test})
    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" test_entry ${test_entry})

    file(STRINGS ${test} suite_name REGEX "^[ ]*TEST_SUITE\\(([A-Za-z_0-9]+)\\)")
    string(REGEX MATCH "TEST_SUITE\\(([A-Za-z_0-9]+)\\)" _ ${suite_name})
    set(suite_name ${CMAKE_MATCH_1})

    if(${test_entry} STREQUAL ${suite_name})
        message(STATUS "Found test entry point.")
    else()
        message(WARNING "Test \"" ${test} "\":\nSuite name must be " ${test_entry} ", not " ${suite_name})
    endif()

    set(part 0)
    foreach(should ${shoulds})
        math(EXPR part "${part} + 1")
        string(REGEX MATCH "\"([^\"]*)\"" should ${should})
        set(should ${CMAKE_MATCH_1})
        string(REGEX REPLACE "[^A-Za-z0-9:/!.,']" "_" should ${should})
        string(REGEX REPLACE "\\.cpp" "" test ${test})
        message("part: " ${part} " " ${should})
        add_test(${test}_${should} ${TEST_PATH}/test_runner ${test} ${part})
    endforeach()

endforeach()

create_test_sourcelist(test_runner_sources test_runner.cpp ${test_sources})
add_executable(test_runner ${test_runner_sources} ${common_sources})
