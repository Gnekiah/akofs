cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0048 NEW)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(akofs_test VERSION 1.0.0 LANGUAGES CXX C)

# add_definitions(-DSPK_DEBUG_OFF)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

include_directories(../src/include/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(pthread pthread_create "" HAVE_LIB_PTHREAD)
if(!HAVE_LIB_PTHREAD)
    MESSAGE(WARNING "libpthread not found!")
endif()
CHECK_LIBRARY_EXISTS(crypto SHA1_Init "" HAVE_LIB_CRYPTO)
if(!HAVE_LIB_CRYPTO)
    MESSAGE(WARNING "libcrypto not found!")
endif()
CHECK_LIBRARY_EXISTS(ssl SSL_in_init "" HAVE_LIB_SSL)
if(!HAVE_LIB_SSL)
    MESSAGE(WARNING "libssl not found!")
endif()
CHECK_LIBRARY_EXISTS(z compress "" HAVE_LIB_Z)
if(!HAVE_LIB_Z)
    MESSAGE(WARNING "zlib not found!")
endif()
CHECK_LIBRARY_EXISTS(uv uv_run "" HAVE_LIB_UV)
if(!HAVE_LIB_UV)
    MESSAGE(WARNING "libuv not found!")
endif()
CHECK_LIBRARY_EXISTS(gtest EXPECT_EQ "" HAVE_LIB_GTEST)
if(!HAVE_LIB_GTEST)
    MESSAGE(WARNING "libgtest not found!")
endif()
CHECK_LIBRARY_EXISTS(gtest_main RUN_ALL_TESTS "" HAVE_LIB_GTEST_MAIN)
if(!HAVE_LIB_GTEST_MAIN)
    MESSAGE(WARNING "libgtest_main not found!")
endif()


CHECK_LIBRARY_EXISTS(pthread pthread_setname_np "" HAVE_FUNC_PTHREAD_SETNAME_NP)
if(HAVE_FUNC_PTHREAD_SETNAME_NP)
    add_definitions(-DHAVE_PTHREAD_SETNAME_NP)
endif()
CHECK_LIBRARY_EXISTS(pthread pthread_getname_np "" HAVE_FUNC_PTHREAD_GETNAME_NP)
if(HAVE_FUNC_PTHREAD_GETNAME_NP)
    add_definitions(-DHAVE_PTHREAD_GETNAME_NP)
endif()
CHECK_LIBRARY_EXISTS(pthread pthread_set_name_np "" HAVE_FUNC_PTHREAD_SET_NAME_NP)
if(HAVE_FUNC_PTHREAD_SET_NAME_NP)
    add_definitions(-DHAVE_PTHREAD_SET_NAME_NP)
endif()
CHECK_LIBRARY_EXISTS(pthread pthread_get_name_np "" HAVE_FUNC_PTHREAD_GET_NAME_NP)
if(HAVE_FUNC_PTHREAD_GET_NAME_NP)
    add_definitions(-DHAVE_PTHREAD_GET_NAME_NP)
endif()


include(CheckIncludeFiles)
CHECK_INCLUDE_FILES(openssl/mdc2.h HAVE_OPENSSL_MDC2_H)
if(HAVE_OPENSSL_MDC2_H)
    add_definitions(-DHAVE_OPENSSL_MDC2_H)
endif()


# include(CheckFunctionExists)
# include(CMakePushCheckState)
# cmake_push_check_state(RESET)
# CHECK_FUNCTION_EXISTS(pthread_set_name_np HAVE_PTHREAD_SET_NAME_NP)
# if(HAVE_PTHREAD_SET_NAME_NP)
#     add_definitions(-DHAVE_PTHREAD_SET_NAME_NP)
# endif()
# CHECK_FUNCTION_EXISTS(pthread_get_name_np HAVE_PTHREAD_GET_NAME_NP)
# if(HAVE_PTHREAD_GET_NAME_NP)
#     add_definitions(-DHAVE_PTHREAD_GET_NAME_NP)
# endif()
# CHECK_FUNCTION_EXISTS(pthread_setname_np HAVE_PTHREAD_SETNAME_NP) 
# if(HAVE_PTHREAD_SETNAME_NP)
#     add_definitions(-DHAVE_PTHREAD_SETNAME_NP)
# endif()
# CHECK_FUNCTION_EXISTS(pthread_getname_np HAVE_PTHREAD_GETNAME_NP)
# if(HAVE_PTHREAD_GETNAME_NP)
#     add_definitions(-DHAVE_PTHREAD_GETNAME_NP)
# endif()
# cmake_pop_check_state()


add_subdirectory(../src/arch/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/arch_objs)
add_subdirectory(../src/common/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/common_objs)
add_subdirectory(../src/daemon/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/daemon_objs)
add_subdirectory(../src/core/ ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/core_objs)



set(TEST_LIBS
    arch_objs
    common_objs
    daemon_objs
    core_objs
    pthread
    crypto
    ssl
    z
    uv
    gtest
    gtest_main)

set(TEST_SRCS 
    main.cpp)

add_executable(akofs_test ${TEST_SRCS})
target_link_libraries(akofs_test ${TEST_LIBS})
