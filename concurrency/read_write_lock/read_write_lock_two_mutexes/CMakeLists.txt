cmake_minimum_required(VERSION 3.26)

project(read_write_lock_two_mutexes)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
    test/test.cpp
    ../../../test/catch_amalgamated.cpp
)

add_executable(read_write_lock_two_mutexes_test ${SOURCE})