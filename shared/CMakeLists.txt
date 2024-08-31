cmake_minimum_required(VERSION 3.19.0)

# Project information
project(Libs VERSION 1.0.0 DESCRIPTION "Shared Lib")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/bin)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/inc)

include(GNUInstallDirs)

add_subdirectory(socket)
add_subdirectory(logger)
add_subdirectory(database)
