cmake_minimum_required(VERSION 3.24)

project(transcoder VERSION 0.0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option (BUILD_TESTS "build tests" ON)

add_subdirectory(transcoder)
if(BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif ()
