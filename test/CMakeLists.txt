cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0048 NEW)
project(sparkle_test VERSION 1.0.0 LANGUAGES CXX C)

#add_definitions(-DSPK_DEBUG_OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../src/include/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_REQUIRED_LIBRARIES pthread)
set(CMAKE_REQUIRED_LIBRARIES crypto)
set(CMAKE_REQUIRED_LIBRARIES ssl)
set(CMAKE_REQUIRED_LIBRARIES gtest)
set(CMAKE_REQUIRED_LIBRARIES gtest_main)
set(CMAKE_REQUIRED_LIBRARIES z)

include(CheckIncludeFiles)
CHECK_INCLUDE_FILES(openssl/mdc2.h HAVE_OPENSSL_MDC2_H)
if(HAVE_OPENSSL_MDC2_H)
    add_definitions(-DHAVE_OPENSSL_MDC2_H)
endif()

include(CheckFunctionExists)
#include(CMakePushCheckState)
#cmake_push_check_state(RESET)
CHECK_FUNCTION_EXISTS(pthread_set_name_np HAVE_PTHREAD_SET_NAME_NP)
if(HAVE_PTHREAD_SET_NAME_NP)
    add_definitions(-DHAVE_PTHREAD_SET_NAME_NP)
endif()
CHECK_FUNCTION_EXISTS(pthread_get_name_np HAVE_PTHREAD_GET_NAME_NP)
if(HAVE_PTHREAD_GET_NAME_NP)
    add_definitions(-DHAVE_PTHREAD_GET_NAME_NP)
endif()
CHECK_FUNCTION_EXISTS(pthread_setname_np HAVE_PTHREAD_SETNAME_NP) 
if(HAVE_PTHREAD_SETNAME_NP)
    add_definitions(-DHAVE_PTHREAD_SETNAME_NP)
endif()
CHECK_FUNCTION_EXISTS(pthread_getname_np HAVE_PTHREAD_GETNAME_NP)
if(HAVE_PTHREAD_GETNAME_NP)
    add_definitions(-DHAVE_PTHREAD_GETNAME_NP)
endif()
#cmake_pop_check_state()

set(TEST_SRCS 
    main.cpp)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

add_subdirectory(../src/arch/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/arch_objs)
add_subdirectory(../src/config/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/config_objs)
add_subdirectory(../src/log/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/log_objs)
add_subdirectory(../src/compto/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/compto_objs)

set(TEST_LIBS
    arch_objs
    config_objs
    log_objs
    compto_objs
    gtest
    gtest_main
    pthread
    crypto
    ssl
    z)

add_executable(sparkle_test ${TEST_SRCS})
target_link_libraries(sparkle_test ${TEST_LIBS})
