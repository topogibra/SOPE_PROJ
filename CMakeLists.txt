cmake_minimum_required(VERSION 3.6)
project(forensic)
set(CMAKE_C_STANDARD 99)

#Uncomment following line to create debug files
set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_FILES src/main.c src/parser.c src/fingerprinter.c src/file.c src/log.c)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -std=c99")
add_executable(forensic ${SOURCE_FILES})
