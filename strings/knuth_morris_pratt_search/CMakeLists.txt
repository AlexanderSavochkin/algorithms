cmake_minimum_required(VERSION 3.26)

project(knuth_morris_pratt)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
    test/test.cpp
    ../../../test/catch_amalgamated.cpp
)

add_executable(knuth_morris_pratt ${SOURCE})