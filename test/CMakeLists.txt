cmake_minimum_required(VERSION 2.8)
project(sparkle_test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../src/include/)

set(TEST_SRCS 
    main.cpp)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else(WIN32)
    add_definitions(-pthread)
endif(WIN32)

add_subdirectory(../src/arch/ arch_objs)

set(TEST_LIBS
    arch_objs
    gtest
    gtest_main)

add_executable(sparkle_test ${TEST_SRCS})
target_link_libraries(sparkle_test ${TEST_LIBS})
