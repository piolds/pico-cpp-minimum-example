project(application)

file(GLOB_RECURSE SourceFiles LIST_DIRECTORIES false *.h *.c *.cpp)

add_executable(${PROJECT_NAME} ${SourceFiles})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
)


target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    common_include
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)


install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/binaries)
