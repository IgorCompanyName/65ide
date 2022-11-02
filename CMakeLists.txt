set(VERSION_MAJOR "0")
set(VERSION_MINOR "2")
set(VERSION_PATCH "1")
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

cmake_policy(SET CMP0048 NEW)
project(ide65 VERSION "${VERSION_STRING}" LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_library(glew include/GL/glew.h src/GL/glew.c)

include_directories(include src)

add_subdirectory(tests)

add_executable(main main.cpp src/mos6502.cpp
    src/event.cpp src/window.cpp src/shader.cpp)

target_link_libraries(main PRIVATE glfw OpenGL::GL glew)

target_include_directories(glew PUBLIC include/)
