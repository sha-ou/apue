cmake_minimum_required(VERSION 3.15)

project(udp)

add_compile_options(-g3)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(rcver rcver.c)
add_executable(snder snder.c)
