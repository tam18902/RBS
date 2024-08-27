cmake_minimum_required(VERSION 3.19.0)

# Project information
project(Socket VERSION 1.0.0 DESCRIPTION "Simple socket library")

# Create the shared library
add_library(Socket SHARED
    socket.cc
)

# Include directory for the library
target_include_directories(Socket PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# set_target_properties(Socket PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(Socket PROPERTIES PUBLIC_HEADER socket.h)

# add_compile_definitions(SOCKET_EXPORT)

# Install the library and header
install(TARGETS Socket
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
