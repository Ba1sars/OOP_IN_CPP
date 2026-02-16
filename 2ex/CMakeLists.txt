cmake_minimum_required(VERSION 3.16)

project(lab2)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_compile_options(--coverage)
add_link_options(--coverage)

add_subdirectory(input)

add_subdirectory(triple_signal)

add_subdirectory(triple_vector)

add_subdirectory(tests)

add_executable(prog prog.cpp)

target_link_libraries(prog triple_signal input)

add_executable(ars_main ars_main.cpp)
target_link_libraries(ars_main triple_vector triple_signal input)
