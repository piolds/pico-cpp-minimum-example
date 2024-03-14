
add_subdirectory(include)
add_subdirectory(src)

if (NOT CMAKE_CROSSCOMPILING) 
    add_subdirectory(test)
endif()
#add_subdirectory(src)
