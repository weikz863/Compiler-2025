cmake_minimum_required(VERSION 3.14)
project(Compiler-2025)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_LEXER_TEST "enable lexer test" OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(project_library src/lexer.cpp src/parse_rules.cpp src/parser.cpp src/parse_tree.cpp)

add_executable(
  main
  src/main.cpp
)

target_link_libraries(
  main
  project_library
)

if(ENABLE_LEXER_TEST)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )

  FetchContent_MakeAvailable(googletest)

  enable_testing()

  add_executable(
    lexer_test
    tests/lexer_test.cpp
  )

  target_link_libraries(
    lexer_test
    project_library
    gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(lexer_test)
endif()