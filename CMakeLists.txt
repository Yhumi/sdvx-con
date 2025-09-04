cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_DIR}/external/pico_sdk_import.cmake)
#include ($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(sdvx C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(sdvx
    sdvx.c
)

pico_enable_stdio_usb(sdvx 1)
pico_enable_stdio_uart(sdvx 1)
pico_add_extra_outputs(sdvx)

target_link_libraries(sdvx pico_stdlib)