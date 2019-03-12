cmake_minimum_required(VERSION 3.6)
project(forensic)
set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/main.c src/fingerprinter.c src/fingerprinter.h)
add_executable(forensic ${SOURCE_FILES})