cmake_minimum_required(VERSION 3.14)
project(Compiler-2025)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(my_library src/lexer.cpp)

enable_testing()

add_executable(
  lexer_test
  tests/lexer_test.cpp
)

target_link_libraries(
  lexer_test
  my_library
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(lexer_test)