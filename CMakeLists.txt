cmake_minimum_required(VERSION 3.27)
project(Lab_1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Lab_1 src/main.c
        src/flags-parser.c
        src/flags-parser.h)
