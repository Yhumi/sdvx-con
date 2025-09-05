cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_DIR}/external/pico_sdk_import.cmake)

project(sdvx C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

pico_sdk_init()

add_subdirectory(src)