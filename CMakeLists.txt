cmake_minimum_required(VERSION 3.6)
project(forensic)
set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/main.c src/parser.c src/fingerprinter.c src/dirtree.c src/file.c src/log.c)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -std=c99 -D_DEFAULT_SOURCE")

#Uncomment following line to create debug files
set(CMAKE_BUILD_TYPE Debug)

add_executable(forensic ${SOURCE_FILES})
