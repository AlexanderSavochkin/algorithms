cmake_minimum_required(VERSION 3.26)

project(preorder_tree_traverse)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
    test/test.cpp
    ../../../test/catch_amalgamated.cpp
)

add_executable(preorder_tree_traverse ${SOURCE})