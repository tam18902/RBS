cmake_minimum_required(VERSION 3.19.0)

# Project information
project(Logger VERSION 1.0.0 DESCRIPTION "Simple Logger library")

# Create the shared library
add_library(Logger SHARED
    logger.cc
)

# Include directory for the library
target_include_directories(Logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(Logger PROPERTIES PUBLIC_HEADER logger.h)

# Install the library and header
install(TARGETS Logger
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
