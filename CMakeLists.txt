cmake_minimum_required(VERSION 3.14)
project(MiniOFP VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(MiniOFP src/main.cpp)

include(FetchContent)

message(STATUS "Fetching GoogleTest...")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest/
  GIT_TAG v1.14.0
)
set(gtests_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(RunGTests tests/test_main_gtest.cpp)

target_link_libraries(RunGTests PRIVATE gtest_main)


# add_executable(RunTests tests/test_main.cpp)

#target_include_directories(RunTests PRIVATE include)
