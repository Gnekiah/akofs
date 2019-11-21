cmake_minimum_required(VERSION 2.8)
project(sparkle_test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(../src/include/)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else(WIN32)
    add_definitions(-pthread)
endif(WIN32)

add_executable(sparkle_test)