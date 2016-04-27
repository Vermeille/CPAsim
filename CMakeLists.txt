cmake_minimum_required(VERSION 2.8)

project(CPASIM CXX)

add_executable(cpasim
    src/expr.h
    src/module.h
    src/parser.cpp
    src/parser.h
    src/wires.h
    src/main.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g3 -Wall -Wextra -Wno-deprecated-declarations")
