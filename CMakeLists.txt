cmake_minimum_required(VERSION 3.17)
project(xasm)

set(CMAKE_CXX_STANDARD 17)

add_executable(xasm src/main.cpp src/token.h src/lexer.cpp src/lexer.h src/parser.cpp src/parser.h src/defs.h src/verifier.cpp src/verifier.h src/XASMGenerator.cpp src/XASMGenerator.h src/encoding.h)