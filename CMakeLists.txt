cmake_minimum_required(VERSION 3.27)
project(cg)

set(CMAKE_CXX_STANDARD 20)
find_package(SFML REQUIRED COMPONENTS Graphics Window System)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

add_subdirectory(lw1)
add_subdirectory(lw2)
add_subdirectory(lw3)
add_subdirectory(lw4)
add_subdirectory(lw5)
add_subdirectory(lw7)
add_subdirectory(lw8)
add_subdirectory(lw10)