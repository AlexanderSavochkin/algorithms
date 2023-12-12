cmake_minimum_required (VERSION 2.8.11)
project(dijkstra)

SET(CMAKE_CXX_STANDARD 20)

set(SOURCE_DIJKSTRA
   ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra.cpp
)

add_executable(dijkstra ${SOURCE_DIJKSTRA})