cmake_minimum_required(VERSION 3.26)

project(qsort_qselect_tests)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
    test/test.cpp
    ../../test/catch_amalgamated.cpp
)

add_executable(qsort_qselect_tests ${SOURCE})