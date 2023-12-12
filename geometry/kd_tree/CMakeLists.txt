cmake_minimum_required(VERSION 3.26)

project(kd_tree_test)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
    test/test.cpp
    ../../test/catch_amalgamated.cpp
)

add_executable(kd_tree_test ${SOURCE})