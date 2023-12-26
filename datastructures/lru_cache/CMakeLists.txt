cmake_minimum_required(VERSION 3.26)

project(lru_cache_test)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
    test/lru_cache_test.cpp
    ../../test/catch_amalgamated.cpp
)

add_executable(lru_cache_test ${SOURCE})