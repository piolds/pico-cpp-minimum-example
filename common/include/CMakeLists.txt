project(common_include)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE hardware_spi)

target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})
