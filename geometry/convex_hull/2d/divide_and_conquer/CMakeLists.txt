cmake_minimum_required(VERSION 3.26)

project(convex_hull_divide_conquer_test)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
    test/test.cpp
    ../../../../test/catch_amalgamated.cpp
)

add_executable(convex_hull_divide_conquer_test ${SOURCE})