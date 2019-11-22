cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0048 NEW)
project(sparkle_test VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../src/include/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CheckFunctionExists)
include(CMakePushCheckState)
cmake_push_check_state(RESET)
set(CMAKE_REQUIRED_LIBRARIES pthread)
CHECK_FUNCTION_EXISTS(pthread_set_name_np HAVE_PTHREAD_SET_NAME_NP)
CHECK_FUNCTION_EXISTS(pthread_get_name_np HAVE_PTHREAD_GET_NAME_NP)
CHECK_FUNCTION_EXISTS(pthread_setname_np HAVE_PTHREAD_SETNAME_NP) 
CHECK_FUNCTION_EXISTS(pthread_getname_np HAVE_PTHREAD_GETNAME_NP)
cmake_pop_check_state()

if(HAVE_PTHREAD_GETNAME_NP)
    add_definitions(-DHAVE_PTHREAD_GETNAME_NP)
endif()
if(HAVE_PTHREAD_SETNAME_NP)
    add_definitions(-DHAVE_PTHREAD_SETNAME_NP)
endif()
if(HAVE_PTHREAD_GET_NAME_NP)
    add_definitions(-DHAVE_PTHREAD_GET_NAME_NP)
endif()
if(HAVE_PTHREAD_SET_NAME_NP)
    add_definitions(-DHAVE_PTHREAD_SET_NAME_NP)
endif()

set(TEST_SRCS 
    main.cpp)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

add_subdirectory(../src/arch/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/arch_objs)
add_subdirectory(../src/config/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/config_objs)

set(TEST_LIBS
    arch_objs
    config_objs
    gtest
    gtest_main
    pthread)

add_executable(sparkle_test ${TEST_SRCS})
target_link_libraries(sparkle_test ${TEST_LIBS})
