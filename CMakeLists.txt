cmake_minimum_required(VERSION 3.15)
project(ok)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(ok main.cpp task.hpp "fileReader.hpp" instanceFactory.hpp fileCommentReader.hpp greedy.hpp)