cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT off)
set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/thirdparty/pico-sdk)  

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(cmake/pico_sdk_import.cmake)

project(pico_cpp_example C CXX ASM)

# initialize the SDK
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/common/include)

add_subdirectory(common)
add_subdirectory(src)

