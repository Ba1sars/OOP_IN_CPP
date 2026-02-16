include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

#find_package(Catch2 REQUIRED)

add_executable(testing testing.cpp)

target_link_libraries(testing triple_signal triple_vector Catch2::Catch2WithMain)
