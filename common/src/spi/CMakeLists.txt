project(spi4)

file(GLOB SourceFiles *.h *.hpp *.cpp)

add_library(${PROJECT_NAME} STATIC  ${SourceFiles})

target_link_libraries(${PROJECT_NAME} common_include)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})


