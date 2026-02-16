cmake_minimum_required (VERSION 3.10)

project(LAB_1)

set(CMAKE_CXX_STANDARD 20)

add_library(generator STATIC generator.hpp generator.cpp)

add_library(dialog STATIC dialog.hpp dialog.cpp)

add_executable(lab1 main.cpp)

target_link_libraries(lab1 PRIVATE dialog generator)

