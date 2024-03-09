cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT off)
set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/pico-sdk)  

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico_cpp_example C CXX ASM)

# initialize the SDK
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# add example code
add_executable(pico_cpp_example src/main.cpp)

pico_enable_stdio_usb(pico_cpp_example 1)
pico_enable_stdio_uart(pico_cpp_example 0)

# add pico_stdlib library which aggregates commonly used features 
target_link_libraries(pico_cpp_example pico_stdlib)

# create extra output files(map/bin/hex).
pico_add_extra_outputs(pico_cpp_example)
