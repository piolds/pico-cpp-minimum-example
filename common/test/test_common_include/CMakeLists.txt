project(test_common_include)

file(GLOB SourceFiles *.h *.cpp)

add_executable(${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME}   
    SSD1306
    common_include
    gtest
    gtest_main
    gmock
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)
