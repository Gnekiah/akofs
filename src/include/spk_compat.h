/*
 * Header definition for handling compatibility.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_COMPAT_H_
#define SPARKLE_SPK_COMPAT_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __GNUC__

#endif

#ifdef _MSC_VER
#endif


#if defined(__GNUC__)
#define SPK_PACK( __Declaration__ ) __Declaration__ \
__attribute__((__packed__))
#elif defined(_MSC_VER)
#define SPK_PACK( __Declaration__ ) __pragma( pack(push, 1) ) \
__Declaration__ __pragma( pack(pop) )
#else
#define SPK_PACK( __Declaration__ ) __Declaration__
#endif


#ifdef _WIN32
#define HAVE_STRUCT_TIMESPEC
#endif

#ifndef MIN
#define MIN(a, b)   (a > b ? b : a)
#endif

#ifndef MAX
#define MAX(a, b)   (a < b ? b : a)
#endif

#include <pthread.h>

#if defined(__GNUC__)
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x)   (x)
#define unlikely(x) (x)
#endif

#if defined(HAVE_PTHREAD_SETNAME_NP)
#define spk_pthread_setname pthread_setname_np
#elif defined(HAVE_PTHREAD_SET_NAME_NP)
#define spk_pthread_setname(thread, name) do { \
    pthread_set_name_np(thread, name); \
    0; \
} while(0);
#else
#define spk_pthread_setname(thread, name) do { \
    0; \
} while(0);
#endif

#if defined(HAVE_PTHREAD_GETNAME_NP)
#define spk_pthread_getname pthread_getname_np
#elif defined(HAVE_PTHREAD_GET_NAME_NP)
#define spk_pthread_getname(thread, name, len) do { \
    pthread_get_name_np(thread, name, len); \
    0; \
} while(0);
#else
#define spk_pthread_getname(thread, name, len) do { \
    if (name != NULL) \
        *name = '\0'; \
    0; \
} while(0);
#endif

// A macro to disallow the copy constructor and operator= functions 
// This should be used in the priavte:declarations for a class
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
    TypeName(const TypeName&);              \
    TypeName& operator=(const TypeName&)
#endif 

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_COMPAT_H_
