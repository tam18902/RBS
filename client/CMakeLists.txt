cmake_minimum_required(VERSION 3.19.0)

# Project information
project(ClientApp VERSION 1.0.0 DESCRIPTION "Client for SimpleSocket")

# Add the client executable
add_executable(client
    src/main.cc
    src/client.cc
)

# Include directory for the client
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_include_directories(client PRIVATE ${CMAKE_INSTALL_INCLUDEDIR})
target_link_directories(client PRIVATE ${CMAKE_INSTALL_LIBDIR})
target_link_libraries(client PRIVATE Socket PRIVATE Logger)
