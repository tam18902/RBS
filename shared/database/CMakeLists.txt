cmake_minimum_required(VERSION 3.19.0)

# Project information
project(Database VERSION 1.0.0 DESCRIPTION "Simple Database library")

# Create the shared library
add_library(Database SHARED
    database.cc
)

# Include directory for the library
target_include_directories(Database PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(Database PROPERTIES PUBLIC_HEADER database.h)

# Install the library and header
install(TARGETS Database
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
