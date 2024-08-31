cmake_minimum_required(VERSION 3.19.0)

# Project information
project(Test VERSION 1.0.0 DESCRIPTION "Test")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test
    main.cc
)

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/inc)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/bin)

target_include_directories(test PRIVATE ${CMAKE_INSTALL_INCLUDEDIR})
target_link_directories(test PRIVATE ${CMAKE_INSTALL_LIBDIR})

target_link_libraries(test PUBLIC Logger Database)
# target_link_libraries(test PUBLIC Database)