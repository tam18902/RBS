cmake_minimum_required(VERSION 3.10)

project(SimpleChatApp VERSION 1.0.0 DESCRIPTION "Simple ChatApp")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_LIBDIR ${CMAKE_SOURCE_DIR}/lib/bin)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_SOURCE_DIR}/lib/inc)

# Add subdirectories
# add_subdirectory(shared)
add_subdirectory(server)
add_subdirectory(client)

# Set the output directories for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
