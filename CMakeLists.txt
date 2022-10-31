project(ide65 C CXX)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_library(glew include/GL/glew.h src/GL/glew.c)

include_directories(include src)

enable_testing()

add_subdirectory(tests)

add_executable(main main.cpp src/mos6502.cpp
    src/event.cpp src/window.cpp src/shader.cpp)

target_link_libraries(main PRIVATE glfw OpenGL::GL glew)

target_include_directories(glew PUBLIC include/)
