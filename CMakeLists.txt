cmake_minimum_required(VERSION 2.8.4)
project(CIA_Auto)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static")

set(SOURCE_FILES main.cpp)
add_executable(CIA_Auto ${SOURCE_FILES})