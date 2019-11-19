cmake_minimum_required(VERSION 3.15)
project(ok)

set(CMAKE_CXX_STANDARD 17)

add_executable(ok main.cpp task.hpp "fileReader.hpp" instanceFactory.hpp fileCommentReader.hpp greedy.hpp)