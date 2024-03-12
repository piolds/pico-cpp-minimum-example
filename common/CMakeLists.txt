
add_subdirectory(include)

if (NOT CMAKE_CROSSCOMPILING) 
    add_subdirectory(test)
endif()
#add_subdirectory(src)
