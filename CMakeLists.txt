cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT off)
set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/thirdparty/pico-sdk)  


if (CMAKE_CROSSCOMPILING)
    # pico_sdk_import.cmake is a single file copied from this SDK 
    # NOTE: this must happen before project()
    include(cmake/pico_sdk_import.cmake)
    # initialize the SDK
    pico_sdk_init()
endif()

project(pico_cpp_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/common/include)
add_subdirectory(common)

if (CMAKE_CROSSCOMPILING)
    add_subdirectory(src)
endif()

if (NOT CMAKE_CROSSCOMPILING)
    # add gtest
    add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/ssd1306)
    include_directories(${PROJECT_NAME} ${ssd1306Dir})
endif()
set(CMSISCORE ${CMAKE_SOURCE_DIR}/thirdparty/cmsis_core/Core)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/CMSIS-DSP)
include_directories(${PROJECT_NAME} ${CMSISDSP})
