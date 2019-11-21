/*
 * Header definition for handling compatibility.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_COMPAT_H_
#define SPARKLE_COMPAT_H_

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
#define spk_pthread_setname(thread, name) ({ \
    pthread_set_name_np(thread, name);          \
    0; })
#else
#define spk_pthread_setname(thread, name) ({ \
    0; })
#endif

#if defined(HAVE_PTHREAD_GETNAME_NP)
#define spk_pthread_getname pthread_getname_np
#elif defined(HAVE_PTHREAD_GET_NAME_NP)
#define spk_pthread_getname(thread, name, len) ({ \
    pthread_get_name_np(thread, name, len);          \
    0; })
#else
#define spk_pthread_getname(thread, name, len) ({ \
    if (name != NULL)                              \
        *name = '\0';                              \
    0; })
#endif

#endif // SPARKLE_COMPAT_H_