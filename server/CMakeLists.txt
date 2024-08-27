cmake_minimum_required(VERSION 3.19.0)

# Project information
project(ServerApp VERSION 1.0.0 DESCRIPTION "Server for SimpleSocket")

# Add the server executable
add_executable(server
    src/server.cc
    src/main.cc
)

# Include directory for the server
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_include_directories(server PRIVATE ${CMAKE_INSTALL_INCLUDEDIR})
target_link_directories(server PRIVATE ${CMAKE_INSTALL_LIBDIR})
target_link_libraries(server PRIVATE Socket PRIVATE Logger)