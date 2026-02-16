cmake_minimum_required(VERSION 3.10)
project(LAB_2 CXX)

include(FetchContent)
include(CTest)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0
)
FetchContent_MakeAvailable(Catch2)

add_library(complex_class STATIC complex_class.hpp complex_class.cpp)
add_library(simple_class STATIC simple_class.hpp simple_class.cpp)
target_include_directories(simple_class PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-O0 -g -fprofile-arcs -ftest-coverage)
  add_link_options(-fprofile-arcs -ftest-coverage)
endif()

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE simple_class Catch2::Catch2WithMain complex_class)
add_test(NAME All_Tests COMMAND tests)

add_executable(lab2 lab2.cpp)
target_link_libraries(lab2 PRIVATE simple_class complex_class)

add_custom_target(coverage
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  COMMAND gcovr
          -r ${CMAKE_SOURCE_DIR}
          --exclude '.*_deps/.*'
          --exclude '.*/CMakeFiles/.*'
          --exclude '${CMAKE_BINARY_DIR}/.*'
          --exclude '.*/Catch2/.*'
          --exclude '.*/catch2-src/.*'
          --exclude '.*lab2.cpp'
          --html-details -o coverage.html
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DEPENDS tests
  VERBATIM
)
