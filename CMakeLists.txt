cmake_minimum_required(VERSION 3.10)

project(ide65)

include_directories(src)

add_executable(main main.cpp src/mos6502.cpp)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)