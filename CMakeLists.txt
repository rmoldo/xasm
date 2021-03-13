cmake_minimum_required(VERSION 3.17)
project(xasm)

set(CMAKE_CXX_STANDARD 17)

add_executable(xasm src/main.cpp src/token.h src/lexer.cpp src/lexer.h src/parser.cpp src/parser.h src/defs.h)