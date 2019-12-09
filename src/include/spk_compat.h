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

/* 
 * package a struct/union/class usage demo
 *  typedef SPK_PACKED(struct spk_pack_t {
 *      uint64_t pos;
 *      size_t size;
 *      uint8_t* buf;
 *  }) spk_pack;
 */
#if defined(__GNUC__)
#define SPK_PACKED( __Declaration__ ) \
__Declaration__ __attribute__((__packed__))
#elif defined(_MSC_VER)
#define SPK_PACKED( __Declaration__ ) \
__pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#else
#define SPK_PACKED( __Declaration__ ) \
__Declaration__
#endif

/*
 * aligning a struct/union/class usage demo
 *  typedef SPK_ALIGNED(struct spk_aligned_t {
 *      char sex;
 *      int len;
 *      char name;
 *  }, 1) spk_aligned;
 *
 *  struct spk_aligned_t {
 *      struct spk_aligned_t* prev, * next;
 *      uint64_t slots;
 *      uintptr_t refcount;
 *      struct spk_aligned_t* page;
 *      SPK_ALIGNED(uint8_t data[], sizeof(void*));
 *  };
 */
#if defined(__GNUC__)
#define SPK_ALIGNED( __Declaration__ , __Aligned__ ) \
__Declaration__ __attribute__((aligned(__Aligned__)))
#elif defined(_MSC_VER)
#define SPK_ALIGNED( __Declaration__ , __Aligned__ ) \
__declspec(align(__Aligned__)) __Declaration__
#else
#define SPK_ALIGNED( __Declaration__ , __Aligned__ ) \
__Declaration__
#endif

/*
 * The static keyword indicates that the function is local to the compiled file. 
 * When it's in a header, it means that it is included in all compiled files. 
 * Then the issue is that if this function is not used, some compilers (clang, 
 * gcc, VS with the proper files) will generate a warning. Tagging the function
 * as unused will remove this warning (and potential error if the warning is 
 * considered as an error).
 */
#if defined(__GNUC__)
#define SPK_UNUSED __attribute__((unused))
#else
#define SPK_UNUSED
#endif

/*
 * pthreads-win32 may internally include time.h (time.h is also commonly included 
 * by other libraries/headers) - and time.h already declares timespec (also, it 
 * does so in a way compatible with pthreads) - yet the pthreads-win32's pthread.h
 * doesn't have the valid include guards for this case (shame on them!). 
 * pthreads tries to declare it because it needs it internally, but since it's 
 * possible it won't need the entire time.h, it tries to declare only the timespec
 * if possible. Still, you can simply add `HAVE_STRUCT_TIMESPEC` before 
 * `#include <pthread.h>` - that will tell the pthreads-win32 header that you 
 * already have a proper timespec, and will let your code compile properly.
 */
#ifdef _WIN32
#define HAVE_STRUCT_TIMESPEC
#endif
#include <pthread.h>

#ifndef MIN
#define MIN(a, b)   (a > b ? b : a)
#endif
#ifndef MAX
#define MAX(a, b)   (a < b ? b : a)
#endif

/*
 * `likely` are hint to the compiler to emit instructions that will cause branch 
 * prediction to favour the `likely` side of a jump instruction. This can be 
 * a big win, if the prediction is correct it means that the jump instruction 
 * is basically free and will take zero cycles. On the other hand if the 
 * prediction is wrong, then it means the processor pipeline needs to be flushed
 * and it can cost several cycles. So long as the prediction is correct most of 
 * the time, this will tend to be good for performance.
 * 
 * Such performance optimisations you should only do it after extensive profiling 
 * to ensure the code really is in a bottleneck, and probably given the micro 
 * nature, that it is being run in a tight loop. 
 */
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
} while(0)
#else
#define spk_pthread_getname(thread, name, len) do { \
    if (name != NULL) \
        *name = '\0'; \
    0; \
} while(0)
#endif

/* 
 * A macro to disallow the copy constructor and operator= functions 
 * This should be used in the priavte:declarations for a class
 */
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)  \
    TypeName(const TypeName&);              \
    TypeName& operator=(const TypeName&)
#endif 

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_COMPAT_H_
