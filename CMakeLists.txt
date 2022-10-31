project(ide65 C CXX)

cmake_minimum_required(VERSION 3.10)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_DIRECTORIES})

include_directories(include src)

add_executable(main main.cpp src/mos6502.cpp src/window.cpp)
