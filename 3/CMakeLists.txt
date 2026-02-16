cmake_minimum_required(VERSION 3.15)
project(MatrixTest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(matrix_tests tests/matr_test.cpp)
target_link_libraries(matrix_tests PRIVATE Catch2::Catch2WithMain)

add_executable(game_main main.cpp 
    Entities/entities.cpp
    GameLogic/game_engine.cpp
    Items/items.cpp
    Map/map.cpp)

target_include_directories(game_main PRIVATE
    ${CMAKE_SOURCE_DIR}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(matrix_tests PRIVATE -g -O0 -Wall -Wextra -pedantic)
    target_compile_options(game_main PRIVATE -g -O0 -Wall -Wextra -pedantic)
endif()
