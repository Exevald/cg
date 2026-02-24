cmake_minimum_required(VERSION 3.27)
project(cg)

set(CMAKE_CXX_STANDARD 20)
find_package(SFML REQUIRED COMPONENTS Graphics Window System)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(lw1)
add_subdirectory(lw2)
add_subdirectory(lw3)