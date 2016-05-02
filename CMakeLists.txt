cmake_minimum_required(VERSION 2.8)

project(CPASIM CXX)

add_executable(cpasim
    src/expr.h
    src/module.h
    src/module.cpp
    src/expr
    src/module
    src/module/modulevalues.cpp
    src/module/modulevalues.h
    src/main.cpp
    src/parser.h
    src/wire
    src/wire/wirevalue.h
    src/wire/wireusage.h
    src/wire/wiredecl.h
    src/parser.cpp
    src/wires.h
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g3 -Wall -Wextra -Wno-deprecated-declarations")
