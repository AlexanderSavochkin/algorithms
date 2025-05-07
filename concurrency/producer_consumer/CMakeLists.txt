cmake_minimum_required(VERSION 3.26)

project(producer_consumer_test)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
    test/test.cpp
)

add_executable(producer_consumer_test ${SOURCE})