/*
 * Builtins and Intrinsics
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_AKO_BUILT_IN_H_
#define AKOFS_AKO_BUILT_IN_H_

#if defined(HEDLEY_GCC_HAS_BUILTIN)
#  define AKO_BUILTIN_GNU_HAS_BUILTIN(builtin,major,minor) \
     HEDLEY_GCC_HAS_BUILTIN(builtin,major,minor,0)
#elif defined(__clang__) && defined(__has_builtin)
#  define AKO_BUILTIN_GNU_HAS_BUILTIN(builtin,major,minor) \
     __has_builtin(builtin)
#elif defined(__GNUC__)
#  define AKO_BUILTIN_GNU_HAS_BUILTIN(builtin,major,minor) \
     (__GNUC__ > major || (major == __GNUC__ && __GNUC_MINOR__ >= minor))
#else
#  define AKO_BUILTIN_GNU_HAS_BUILTIN(builtin,major,minor) (0)
#endif

#if defined(HEDLEY_CLANG_HAS_BUILTIN)
#  define AKO_BUILTIN_CLANG_HAS_BUILTIN(builtin) \
     HEDLEY_CLANG_HAS_BUILTIN(builtin)
#elif defined(__has_builtin)
#  define AKO_BUILTIN_CLANG_HAS_BUILTIN(builtin) \
     __has_builtin(builtin)
#else
#  define AKO_BUILTIN_CLANG_HAS_BUILTIN(builtin) (0)
#endif

#if defined(HEDLEY_MSVC_VERSION_CHECK)
#  define AKO_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) \
     HEDLEY_MSVC_VERSION_CHECK(major,minor,0)
#elif !defined(_MSC_VER)
#  define AKO_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) (0)
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define AKO_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) \
     (_MSC_FULL_VER >= ((major * 1000000) + (minor * 10000)))
#elif defined(_MSC_VER) && (_MSC_VER >= 1200)
#  define AKO_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) \
     (_MSC_FULL_VER >= ((major * 100000) + (minor * 1000)))
#else
#  define AKO_BUILTIN_MSVC_HAS_INTRIN(intrin,major,minor) \
     (_MSC_VER >= ((major * 100) + (minor)))
#endif

#if defined(_MSC_VER)
#  include <intrin.h>
#endif
#include <limits.h>
#include <stdlib.h>

#if defined(__i386) || defined(_M_IX86) || \
  defined(__amd64) || defined(_M_AMD64) || defined(__x86_64)
#  if defined(_MSC_VER)
#    define AKO_BUILTIN__ENABLE_X86
#  elif defined(__GNUC__)
#    define AKO_BUILTIN__ENABLE_X86
#    include <x86intrin.h>
#  endif
#endif

#if defined(__amd64) || defined(_M_AMD64) || defined(__x86_64)
#  if defined(_MSC_VER)
#    define AKO_BUILTIN__ENABLE_AMD64
#  elif defined(__GNUC__)
#    define AKO_BUILTIN__ENABLE_AMD64
#    include <x86intrin.h>
#  endif
#endif

#if defined(__ARM_ACLE)
#  include <arm_acle.h>
#endif

/* 
 * For maximum portability include the exact-int module from portable snippets. 
 */
#if \
  !defined(ako_int64_t) || !defined(ako_uint64_t) || \
  !defined(ako_int32_t) || !defined(ako_uint32_t) || \
  !defined(ako_int16_t) || !defined(ako_uint16_t) || \
  !defined(ako_int8_t)  || !defined(ako_uint8_t)
#  include <stdint.h>
#  if !defined(ako_int64_t)
#    define ako_int64_t int64_t
#  endif
#  if !defined(ako_uint64_t)
#    define ako_uint64_t uint64_t
#  endif
#  if !defined(ako_int32_t)
#    define ako_int32_t int32_t
#  endif
#  if !defined(ako_uint32_t)
#    define ako_uint32_t uint32_t
#  endif
#  if !defined(ako_int16_t)
#    define ako_int16_t int16_t
#  endif
#  if !defined(ako_uint16_t)
#    define ako_uint16_t uint16_t
#  endif
#  if !defined(ako_int8_t)
#    define ako_int8_t int8_t
#  endif
#  if !defined(ako_uint8_t)
#    define ako_uint8_t uint8_t
#  endif
#endif

#if defined(HEDLEY_LIKELY) && defined(HEDLEY_UNLIKELY)
#  define AKO_BUILTIN_LIKELY(expr) HEDLEY_LIKELY(expr)
#  define AKO_BUILTIN_UNLIKELY(expr) HEDLEY_UNLIKELY(expr)
#elif AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_expect,3,0)
#  define AKO_BUILTIN_LIKELY(expr) __builtin_expect(!!(expr), 1)
#  define AKO_BUILTIN_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#else
#  define AKO_BUILTIN_LIKELY(expr) (!!(expr))
#  define AKO_BUILTIN_UNLIKELY(expr) (!!(expr))
#endif

#if !defined(AKO_BUILTIN_STATIC_INLINE)
#  if defined(__GNUC__)
#    define AKO_BUILTIN__COMPILER_ATTRIBUTES __attribute__((__unused__))
#  else
#    define AKO_BUILTIN__COMPILER_ATTRIBUTES
#  endif

#  if defined(HEDLEY_INLINE)
#    define AKO_BUILTIN__INLINE HEDLEY_INLINE
#  elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#    define AKO_BUILTIN__INLINE inline
#  elif defined(__GNUC_STDC_INLINE__)
#    define AKO_BUILTIN__INLINE __inline__
#  elif defined(_MSC_VER) && _MSC_VER >= 1200
#    define AKO_BUILTIN__INLINE __inline
#  else
#    define AKO_BUILTIN__INLINE
#  endif

#  define AKO_BUILTIN__FUNCTION AKO_BUILTIN__COMPILER_ATTRIBUTES \
     static AKO_BUILTIN__INLINE
#endif

#define AKO_BUILTIN__SUFFIX_B  1
#define AKO_BUILTIN__SUFFIX_S  2
#define AKO_BUILTIN__SUFFIX_   3
#define AKO_BUILTIN__SUFFIX_L  4
#define AKO_BUILTIN__SUFFIX_LL 5

#if !defined(AKO_BUILTIN__SIZEOF_CHAR)
#  if   CHAR_MIN == (-0x7fLL-1) && CHAR_MAX == 0x7fLL
#    define AKO_BUILTIN__SIZEOF_CHAR 8
#  elif CHAR_MIN == (-0x7fffLL-1) && CHAR_MAX == 0x7fffLL
#    define AKO_BUILTIN__SIZEOF_CHAR 16
#  elif CHAR_MIN == (-0x7fffffffLL-1) && CHAR_MAX == 0x7fffffffLL
#    define AKO_BUILTIN__SIZEOF_CHAR 32
#  elif CHAR_MIN == (-0x7fffffffffffffffLL-1) && CHAR_MAX == 0x7fffffffffffffffLL
#    define AKO_BUILTIN__SIZEOF_CHAR 64
#  endif
#endif

#if !defined(AKO_BUILTIN__SIZEOF_SHRT)
#  if   SHRT_MIN == (-0x7fLL-1) && SHRT_MAX == 0x7fLL
#    define AKO_BUILTIN__SIZEOF_SHRT 8
#  elif SHRT_MIN == (-0x7fffLL-1) && SHRT_MAX == 0x7fffLL
#    define AKO_BUILTIN__SIZEOF_SHRT 16
#  elif SHRT_MIN == (-0x7fffffffLL-1) && SHRT_MAX == 0x7fffffffLL
#    define AKO_BUILTIN__SIZEOF_SHRT 32
#  elif SHRT_MIN == (-0x7fffffffffffffffLL-1) && SHRT_MAX == 0x7fffffffffffffffLL
#    define AKO_BUILTIN__SIZEOF_SHRT 64
#  endif
#endif

#if !defined(AKO_BUILTIN__SIZEOF_INT)
#  if   INT_MIN == (-0x7fLL-1) && INT_MAX == 0x7fLL
#    define AKO_BUILTIN__SIZEOF_INT 8
#  elif INT_MIN == (-0x7fffLL-1) && INT_MAX == 0x7fffLL
#    define AKO_BUILTIN__SIZEOF_INT 16
#  elif INT_MIN == (-0x7fffffffLL-1) && INT_MAX == 0x7fffffffLL
#    define AKO_BUILTIN__SIZEOF_INT 32
#  elif INT_MIN == (-0x7fffffffffffffffLL-1) && INT_MAX == 0x7fffffffffffffffLL
#    define AKO_BUILTIN__SIZEOF_INT 64
#  endif
#endif

#if !defined(AKO_BUILTIN__SIZEOF_LONG)
#  if   LONG_MIN == (-0x7fLL-1) && LONG_MAX == 0x7fLL
#    define AKO_BUILTIN__SIZEOF_LONG 8
#  elif LONG_MIN == (-0x7fffLL-1) && LONG_MAX == 0x7fffLL
#    define AKO_BUILTIN__SIZEOF_LONG 16
#  elif LONG_MIN == (-0x7fffffffLL-1) && LONG_MAX == 0x7fffffffLL
#    define AKO_BUILTIN__SIZEOF_LONG 32
#  elif LONG_MIN == (-0x7fffffffffffffffLL-1) && LONG_MAX == 0x7fffffffffffffffLL
#    define AKO_BUILTIN__SIZEOF_LONG 64
#  endif
#endif

#if !defined(AKO_BUILTIN__SIZEOF_LLONG)
#  if   LLONG_MIN == (-0x7fLL-1) && LLONG_MAX == 0x7fLL
#    define AKO_BUILTIN__SIZEOF_LLONG 8
#  elif LLONG_MIN == (-0x7fffLL-1) && LLONG_MAX == 0x7fffLL
#    define AKO_BUILTIN__SIZEOF_LLONG 16
#  elif LLONG_MIN == (-0x7fffffffLL-1) && LLONG_MAX == 0x7fffffffLL
#    define AKO_BUILTIN__SIZEOF_LLONG 32
#  elif LLONG_MIN == (-0x7fffffffffffffffLL-1) && LLONG_MAX == 0x7fffffffffffffffLL
#    define AKO_BUILTIN__SIZEOF_LLONG 64
#  endif
#endif

#if !defined(AKO_BUILTIN_SUFFIX_INT8)
#  if CHAR_BIT == 8 || AKO_BUILTIN__SIZEOF_CHAR == 8
#    define AKO_BUILTIN_SUFFIX_INT8 AKO_BUILTIN__SUFFIX_B
#  elif AKO_BUILTIN__SIZEOF_SHRT == 8
#    define AKO_BUILTIN_SUFFIX_INT8 AKO_BUILTIN__SUFFIX_S
#  elif AKO_BUILTIN__SIZEOF_INT == 8
#    define AKO_BUILTIN_SUFFIX_INT8 AKO_BUILTIN__SUFFIX_
#  elif AKO_BUILTIN__SIZEOF_LONG == 8
#    define AKO_BUILTIN_SUFFIX_INT8 AKO_BUILTIN__SUFFIX_L
#  elif AKO_BUILTIN__SIZEOF_LLONG == 8
#    define AKO_BUILTIN_SUFFIX_INT8 AKO_BUILTIN__SUFFIX_LL
#  endif
#endif

#if !defined(AKO_BUILTIN_SUFFIX_INT16)
#  if AKO_BUILTIN__SIZEOF_CHAR == 16
#    define AKO_BUILTIN_SUFFIX_INT16 AKO_BUILTIN__SUFFIX_B
#  elif AKO_BUILTIN__SIZEOF_SHRT == 16
#    define AKO_BUILTIN_SUFFIX_INT16 AKO_BUILTIN__SUFFIX_S
#  elif AKO_BUILTIN__SIZEOF_INT == 16
#    define AKO_BUILTIN_SUFFIX_INT16 AKO_BUILTIN__SUFFIX_
#  elif AKO_BUILTIN__SIZEOF_LONG == 16
#    define AKO_BUILTIN_SUFFIX_INT16 AKO_BUILTIN__SUFFIX_L
#  elif AKO_BUILTIN__SIZEOF_LLONG == 16
#    define AKO_BUILTIN_SUFFIX_INT16 AKO_BUILTIN__SUFFIX_LL
#  endif
#endif

#if !defined(AKO_BUILTIN_SUFFIX_INT32)
#  if AKO_BUILTIN__SIZEOF_CHAR == 32
#    define AKO_BUILTIN_SUFFIX_INT32 AKO_BUILTIN__SUFFIX_B
#  elif AKO_BUILTIN__SIZEOF_SHRT == 32
#    define AKO_BUILTIN_SUFFIX_INT32 AKO_BUILTIN__SUFFIX_S
#  elif AKO_BUILTIN__SIZEOF_INT == 32
#    define AKO_BUILTIN_SUFFIX_INT32 AKO_BUILTIN__SUFFIX_
#  elif AKO_BUILTIN__SIZEOF_LONG == 32
#    define AKO_BUILTIN_SUFFIX_INT32 AKO_BUILTIN__SUFFIX_L
#  elif AKO_BUILTIN__SIZEOF_LLONG == 32
#    define AKO_BUILTIN_SUFFIX_INT32 AKO_BUILTIN__SUFFIX_LL
#  endif
#endif

#if !defined(AKO_BUILTIN_SUFFIX_INT64)
#  if defined(__APPLE__) && AKO_BUILTIN__SIZEOF_LLONG == 64
#    define AKO_BUILTIN_SUFFIX_INT64 AKO_BUILTIN__SUFFIX_LL
#  elif AKO_BUILTIN__SIZEOF_CHAR == 64
#    define AKO_BUILTIN_SUFFIX_INT64 AKO_BUILTIN__SUFFIX_B
#  elif AKO_BUILTIN__SIZEOF_SHRT == 64
#    define AKO_BUILTIN_SUFFIX_INT64 AKO_BUILTIN__SUFFIX_S
#  elif AKO_BUILTIN__SIZEOF_INT == 64
#    define AKO_BUILTIN_SUFFIX_INT64 AKO_BUILTIN__SUFFIX_
#  elif AKO_BUILTIN__SIZEOF_LONG == 64
#    define AKO_BUILTIN_SUFFIX_INT64 AKO_BUILTIN__SUFFIX_L
#  elif AKO_BUILTIN__SIZEOF_LLONG == 64
#    define AKO_BUILTIN_SUFFIX_INT64 AKO_BUILTIN__SUFFIX_LL
#  endif
#endif

#if defined(AKO_BUILTIN_SUFFIX_INT8)
#  if AKO_BUILTIN_SUFFIX_INT8 == 1
#    define AKO_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name##b
#  elif AKO_BUILTIN_SUFFIX_INT8 == 2
#    define AKO_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name##s
#  elif AKO_BUILTIN_SUFFIX_INT8 == 3
#    define AKO_BUILTIN__VARIANT_INT8(prefix,name)  prefix##_builtin_##name
#    define AKO_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name
#  elif AKO_BUILTIN_SUFFIX_INT8 == 4
#    define AKO_BUILTIN__VARIANT_INT8(prefix,name)  prefix##_builtin_##name##l
#    define AKO_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name##l
#  elif AKO_BUILTIN_SUFFIX_INT8 == 5
#    define AKO_BUILTIN__VARIANT_INT8(prefix,name)  prefix##_builtin_##name##ll
#    define AKO_BUILTIN__VARIANT2_INT8(prefix,name) prefix##_builtin_##name##ll
#  endif
#endif

#if defined(AKO_BUILTIN_SUFFIX_INT16)
#  if AKO_BUILTIN_SUFFIX_INT16 == 1
#    define AKO_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name##b
#  elif AKO_BUILTIN_SUFFIX_INT16 == 2
#    define AKO_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name##s
#  elif AKO_BUILTIN_SUFFIX_INT16 == 3
#    define AKO_BUILTIN__VARIANT_INT16(prefix,name)  prefix##_builtin_##name
#    define AKO_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name
#  elif AKO_BUILTIN_SUFFIX_INT16 == 4
#    define AKO_BUILTIN__VARIANT_INT16(prefix,name)  prefix##_builtin_##name##l
#    define AKO_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name##l
#  elif AKO_BUILTIN_SUFFIX_INT16 == 5
#    define AKO_BUILTIN__VARIANT_INT16(prefix,name)  prefix##_builtin_##name##ll
#    define AKO_BUILTIN__VARIANT2_INT16(prefix,name) prefix##_builtin_##name##ll
#  endif
#endif

#if defined(AKO_BUILTIN_SUFFIX_INT32)
#  if AKO_BUILTIN_SUFFIX_INT32 == 1
#    define AKO_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name##b
#  elif AKO_BUILTIN_SUFFIX_INT32 == 2
#    define AKO_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name##s
#  elif AKO_BUILTIN_SUFFIX_INT32 == 3
#    define AKO_BUILTIN__VARIANT_INT32(prefix,name)  prefix##_builtin_##name
#    define AKO_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name
#  elif AKO_BUILTIN_SUFFIX_INT32 == 4
#    define AKO_BUILTIN__VARIANT_INT32(prefix,name)  prefix##_builtin_##name##l
#    define AKO_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name##l
#  elif AKO_BUILTIN_SUFFIX_INT32 == 5
#    define AKO_BUILTIN__VARIANT_INT32(prefix,name)  prefix##_builtin_##name##ll
#    define AKO_BUILTIN__VARIANT2_INT32(prefix,name) prefix##_builtin_##name##ll
#  endif
#endif

#if defined(AKO_BUILTIN_SUFFIX_INT64)
#  if AKO_BUILTIN_SUFFIX_INT64 == 1
#    define AKO_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name##b
#  elif AKO_BUILTIN_SUFFIX_INT64 == 2
#    define AKO_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name##s
#  elif AKO_BUILTIN_SUFFIX_INT64 == 3
#    define AKO_BUILTIN__VARIANT_INT64(prefix,name)  prefix##_builtin_##name
#    define AKO_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name
#  elif AKO_BUILTIN_SUFFIX_INT64 == 4
#    define AKO_BUILTIN__VARIANT_INT64(prefix,name)  prefix##_builtin_##name##l
#    define AKO_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name##l
#  elif AKO_BUILTIN_SUFFIX_INT64 == 5
#    define AKO_BUILTIN__VARIANT_INT64(prefix,name)  prefix##_builtin_##name##ll
#    define AKO_BUILTIN__VARIANT2_INT64(prefix,name) prefix##_builtin_##name##ll
#  endif
#endif

     /******
      *** GCC-style built-ins
      ******/

      /*** __builtin_ffs ***/

#define AKO_BUILTIN__FFS_DEFINE_PORTABLE(f_n, T)	\
  AKO_BUILTIN__FUNCTION				\
  int ako_builtin_##f_n(T x) {			\
    static const char ako_builtin_ffs_lookup[256] = { \
      0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      8, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,	\
      5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1	\
    };							\
							\
    unsigned char t;					\
    size_t s = 0;					\
							\
    while (s < (sizeof(T) * 8)) {			\
      t = (unsigned char) ((x >> s) & 0xff);		\
      if (t)						\
        return ako_builtin_ffs_lookup[t] + s;		\
							\
      s += 8;						\
    }							\
							\
    return 0;						\
  }

#if AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_ffs, 3, 3)
#  define ako_builtin_ffs(x)   __builtin_ffs(x)
#  define ako_builtin_ffsl(x)  __builtin_ffsl(x)
#  define ako_builtin_ffsll(x) __builtin_ffsll(x)
#  define ako_builtin_ffs32(x) AKO_BUILTIN__VARIANT_INT32(_,ffs)(x)
#  define ako_builtin_ffs64(x) AKO_BUILTIN__VARIANT_INT64(_,ffs)(x)
#else
#  if AKO_BUILTIN_MSVC_HAS_INTRIN(_BitScanForward, 14, 0)
AKO_BUILTIN__FUNCTION
int ako_builtin_ffsll(long long v) {
    unsigned long r;
#    if defined(_M_AMD64) || defined(_M_ARM)
    if (_BitScanForward64(&r, (unsigned long long) v)) {
        return (int)(r + 1);
    }
#    else
    if (_BitScanForward(&r, (unsigned long)(v))) {
        return (int)(r + 1);
    }
    else if (_BitScanForward(&r, (unsigned long)(v >> 32))) {
        return (int)(r + 33);
    }
#    endif
    return 0;
}

AKO_BUILTIN__FUNCTION
int ako_builtin_ffsl(long v) {
    unsigned long r;
    if (_BitScanForward(&r, (unsigned long)v)) {
        return (int)(r + 1);
    }
    return 0;
}

AKO_BUILTIN__FUNCTION
int ako_builtin_ffs(int v) {
    return ako_builtin_ffsl(v);
}
#  else
AKO_BUILTIN__FFS_DEFINE_PORTABLE(ffs, int)
AKO_BUILTIN__FFS_DEFINE_PORTABLE(ffsl, long)
AKO_BUILTIN__FFS_DEFINE_PORTABLE(ffsll, long long)
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_ffsll(v) ako_builtin_ffsll(v)
#    define __builtin_ffsl(v)  ako_builtin_ffsl(v)
#    define __builtin_ffs(v)   ako_builtin_ffs(v)
#  endif
#endif

#if !defined(ako_builtin_ffs32)
#  define ako_builtin_ffs32(x) AKO_BUILTIN__VARIANT_INT32(ako,ffs)(x)
#endif

#if !defined(ako_builtin_ffs64)
#  define ako_builtin_ffs64(x) AKO_BUILTIN__VARIANT_INT64(ako,ffs)(x)
#endif

/*** __builtin_clz ***/

#define AKO_BUILTIN__CLZ_DEFINE_PORTABLE(f_n, T)	\
  AKO_BUILTIN__FUNCTION				\
  int ako_builtin_##f_n(T x) {			\
    static const char ako_builtin_clz_lookup[256] = {	\
      7, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,	\
      3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,	\
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	\
      2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	\
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	\
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	\
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	\
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	\
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	\
    };							\
    size_t s = sizeof(T) * 8;				\
    T r;						\
							\
    while ((s -= 8) != 0) {				\
      r = x >> s;					\
      if (r != 0)					\
        return ako_builtin_clz_lookup[r] +		\
          (((sizeof(T) - 1) * 8) - s);			\
    }							\
							\
    if (x == 0)						\
      return (int) ((sizeof(T) * 8) - 1);		\
    else						\
      return ako_builtin_clz_lookup[x] +		\
        ((sizeof(T) - 1) * 8);				\
  }

#if AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_clz, 3, 4)
#  define ako_builtin_clz(x)   __builtin_clz(x)
#  define ako_builtin_clzl(x)  __builtin_clzl(x)
#  define ako_builtin_clzll(x) __builtin_clzll(x)
#  define ako_builtin_clz32(x) AKO_BUILTIN__VARIANT_INT32(_,clz)(x)
#  define ako_builtin_clz64(x) AKO_BUILTIN__VARIANT_INT64(_,clz)(x)
#else
#  if AKO_BUILTIN_MSVC_HAS_INTRIN(_BitScanReverse,14,0)
AKO_BUILTIN__FUNCTION
int ako_builtin_clzll(unsigned long long v) {
    unsigned long r = 0;
#    if defined(_M_AMD64) || defined(_M_ARM)
    if (_BitScanReverse64(&r, v)) {
        return 63 - r;
    }
#    else
    if (_BitScanReverse(&r, (unsigned long)(v >> 32))) {
        return 31 - r;
    }
    else if (_BitScanReverse(&r, (unsigned long)v)) {
        return 63 - r;
    }
#    endif
    return 63;
}

AKO_BUILTIN__FUNCTION
int ako_builtin_clzl(unsigned long v) {
    unsigned long r = 0;
    if (_BitScanReverse(&r, v)) {
        return 31 - r;
    }
    return 31;
}

AKO_BUILTIN__FUNCTION
int ako_builtin_clz(unsigned int v) {
    return ako_builtin_clzl(v);
}
#    define ako_builtin_clz32(x) AKO_BUILTIN__VARIANT_INT32(ako,clz)(x)
#    define ako_builtin_clz64(x) AKO_BUILTIN__VARIANT_INT64(ako,clz)(x)
#  else
AKO_BUILTIN__FUNCTION
int ako_builtin_clz32(ako_uint32_t v) {
    static const unsigned char MultiplyDeBruijnBitPosition[] = {
      0,  9,  1, 10, 13, 21,  2, 29, 11, 14, 16, 18, 22, 25,  3, 30,
      8, 12, 20, 28, 15, 17, 24,  7, 19, 27, 23,  6, 26,  5,  4, 31
    };

    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    return
        ((sizeof(ako_uint32_t) * CHAR_BIT) - 1) -
        MultiplyDeBruijnBitPosition[(ako_uint32_t)(v * 0x07C4ACDDU) >> 27];
}

AKO_BUILTIN__FUNCTION
int ako_builtin_clz64(ako_uint64_t v) {
    static const unsigned char MultiplyDeBruijnBitPosition[] = {
       0, 47,  1, 56, 48, 27,  2, 60, 57, 49, 41, 37, 28, 16,  3, 61,
      54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11,  4, 62,
      46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45,
      25, 39, 14, 33, 19, 30,  9, 24, 13, 18,  8, 12,  7,  6,  5, 63
    };

    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;

    return
        ((sizeof(ako_uint64_t) * CHAR_BIT) - 1) -
        MultiplyDeBruijnBitPosition[(ako_uint64_t)(v * 0x03F79D71B4CB0A89ULL) >> 58];
}

#    if AKO_BUILTIN__SIZEOF_INT == 32
AKO_BUILTIN__FUNCTION int ako_builtin_clz(unsigned int x) \
  { return ako_builtin_clz32(x); }
#    elif AKO_BUILTIN__SIZEOF_INT == 64
AKO_BUILTIN__FUNCTION int ako_builtin_clz(unsigned int x) \
  { return ako_builtin_clz64(x); }
#    else
AKO_BUILTIN__CLZ_DEFINE_PORTABLE(clz, unsigned int)
#    endif

#    if AKO_BUILTIN__SIZEOF_LONG == 32
AKO_BUILTIN__FUNCTION int ako_builtin_clzl(unsigned long x) \
  { return ako_builtin_clz32(x); }
#    elif AKO_BUILTIN__SIZEOF_LONG == 64
AKO_BUILTIN__FUNCTION int ako_builtin_clzl(unsigned long x) \
  { return ako_builtin_clz64(x); }
#    else
AKO_BUILTIN__CLZ_DEFINE_PORTABLE(clzl, unsigned long)
#    endif

#    if AKO_BUILTIN__SIZEOF_LLONG == 32
AKO_BUILTIN__FUNCTION int ako_builtin_clzll(unsigned long long x) \
  { return ako_builtin_clz32(x); }
#    elif AKO_BUILTIN__SIZEOF_LLONG == 64
AKO_BUILTIN__FUNCTION int ako_builtin_clzll(unsigned long long x) \
  { return ako_builtin_clz64(x); }
#    else
AKO_BUILTIN__CLZ_DEFINE_PORTABLE(clzll, unsigned long long)
#    endif

#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_clz(x)   ako_builtin_clz(x)
#    define __builtin_clzl(x)  ako_builtin_clzl(x)
#    define __builtin_clzll(x) ako_builtin_clzll(x)
#  endif
#endif

#if !defined(ako_builtin_clz32)
#  define ako_builtin_clz32(x) AKO_BUILTIN__VARIANT_INT32(ako,clz)(x)
#endif

#if !defined(ako_builtin_clz64)
#  define ako_builtin_clz64(x) AKO_BUILTIN__VARIANT_INT64(ako,clz)(x)
#endif

/*** __builtin_ctz ***/

#define AKO_BUILTIN__CTZ_DEFINE_PORTABLE(f_n, T)	\
  AKO_BUILTIN__FUNCTION				\
  int ako_builtin_##f_n(T x) {			\
    static const char ako_builtin_ctz_lookup[256] = {	\
      0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,	\
      4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0	\
    };							\
    size_t s = 0;					\
    T r;						\
							\
    do {						\
      r = (x >> s) & 0xff;				\
      if (r != 0)					\
        return ako_builtin_ctz_lookup[r] + (char) s;	\
    } while ((s += 8) < (sizeof(T) * 8));		\
							\
    return (int) sizeof(T) - 1;				\
  }

#if AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_ctz, 3, 4)
#  define ako_builtin_ctz(x)   __builtin_ctz(x)
#  define ako_builtin_ctzl(x)  __builtin_ctzl(x)
#  define ako_builtin_ctzll(x) __builtin_ctzll(x)
#  define ako_builtin_ctz32(x) AKO_BUILTIN__VARIANT_INT32(_,ctz)(x)
#  define ako_builtin_ctz64(x) AKO_BUILTIN__VARIANT_INT64(_,ctz)(x)
#else
#  if AKO_BUILTIN_MSVC_HAS_INTRIN(_BitScanForward, 14, 0)
    AKO_BUILTIN__FUNCTION
    int ako_builtin_ctzll(unsigned long long v) {
    unsigned long r = 0;
#    if defined(_M_AMD64) || defined(_M_ARM)
    _BitScanForward64(&r, v);
    return (int)r;
#    else
    if (_BitScanForward(&r, (unsigned int)(v)))
        return (int)(r);

    _BitScanForward(&r, (unsigned int)(v >> 32));
    return (int)(r + 32);
#    endif
}

AKO_BUILTIN__FUNCTION
int ako_builtin_ctzl(unsigned long v) {
    unsigned long r = 0;
    _BitScanForward(&r, v);
    return (int)r;
}

AKO_BUILTIN__FUNCTION
int ako_builtin_ctz(unsigned int v) {
    return ako_builtin_ctzl(v);
}
#    define ako_builtin_ctz32(x) AKO_BUILTIN__VARIANT_INT32(ako,ctz)(x)
#    define ako_builtin_ctz64(x) AKO_BUILTIN__VARIANT_INT64(ako,ctz)(x)
#  else
    AKO_BUILTIN__FUNCTION
    int ako_builtin_ctz32(ako_uint32_t v) {
    static const unsigned char MultiplyDeBruijnBitPosition[] = {
       0,  1, 28,  2, 29, 14, 24,  3, 30, 22, 20, 15, 25, 17,  4,  8,
      31, 27, 13, 23, 21, 19, 16,  7, 26, 12, 18,  6, 11,  5, 10,  9
    };

    return
        MultiplyDeBruijnBitPosition[((ako_uint32_t)((v & -v) * 0x077CB531U)) >> 27];
}

AKO_BUILTIN__FUNCTION
int ako_builtin_ctz64(ako_uint64_t v) {
    static const unsigned char MultiplyDeBruijnBitPosition[] = {
       0,  1, 56,  2, 57, 49, 28,  3, 61, 58, 42, 50, 38, 29, 17,  4,
      62, 47, 59, 36, 45, 43, 51, 22, 53, 39, 33, 30, 24, 18, 12,  5,
      63, 55, 48, 27, 60, 41, 37, 16, 46, 35, 44, 21, 52, 32, 23, 11,
      54, 26, 40, 15, 34, 20, 31, 10, 25, 14, 19,  9, 13,  8,  7,  6
    };

    return
        MultiplyDeBruijnBitPosition[((ako_uint64_t)((v & -v) * 0x03f79d71b4ca8b09ULL)) >> 58];
}

#    if AKO_BUILTIN__SIZEOF_INT == 32
AKO_BUILTIN__FUNCTION int ako_builtin_ctz(unsigned int x) \
  { return ako_builtin_ctz32(x); }
#    elif AKO_BUILTIN__SIZEOF_INT == 64
AKO_BUILTIN__FUNCTION int ako_builtin_ctz(unsigned int x) \
  { return ako_builtin_ctz64(x); }
#    else
AKO_BUILTIN__CTZ_DEFINE_PORTABLE(ctz, unsigned int)
#    endif

#    if AKO_BUILTIN__SIZEOF_LONG == 32
AKO_BUILTIN__FUNCTION int ako_builtin_ctzl(unsigned long x) \
  { return ako_builtin_ctz32(x); }
#    elif AKO_BUILTIN__SIZEOF_LONG == 64
AKO_BUILTIN__FUNCTION int ako_builtin_ctzl(unsigned long x) \
  { return ako_builtin_ctz64(x); }
#    else
AKO_BUILTIN__CTZ_DEFINE_PORTABLE(ctzl, unsigned long)
#    endif

#    if AKO_BUILTIN__SIZEOF_LLONG == 32
AKO_BUILTIN__FUNCTION int ako_builtin_ctzll(unsigned long long x) \
  { return ako_builtin_ctz32(x); }
#    elif AKO_BUILTIN__SIZEOF_LLONG == 64
AKO_BUILTIN__FUNCTION int ako_builtin_ctzll(unsigned long long x) \
  { return ako_builtin_ctz64(x); }
#    else
AKO_BUILTIN__CTZ_DEFINE_PORTABLE(ctzll, unsigned long long)
#    endif
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_ctz(x)   ako_builtin_ctz(x)
#    define __builtin_ctzl(x)  ako_builtin_ctzl(x)
#    define __builtin_ctzll(x) ako_builtin_ctzll(x)
#  endif
#endif

#if !defined(ako_builtin_ctz32)
#  define ako_builtin_ctz32(x) AKO_BUILTIN__VARIANT_INT32(ako,ctz)(x)
#endif

#if !defined(ako_builtin_ctz64)
#  define ako_builtin_ctz64(x) AKO_BUILTIN__VARIANT_INT64(ako,ctz)(x)
#endif

/*** __builtin_parity ***/

#define AKO_BUILTIN__PARITY_DEFINE_PORTABLE(f_n, T)			\
  AKO_BUILTIN__FUNCTION						\
  int ako_builtin_##f_n(T v) {					\
    size_t i;								\
    for (i = (sizeof(T) * CHAR_BIT) / 2 ; i > 2 ; i /= 2)		\
      v ^= v >> i;							\
    v &= 0xf;								\
    return (0x6996 >> v) & 1;						\
  }

#if AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_parity, 3, 4)
#  define ako_builtin_parity(x)   __builtin_parity(x)
#  define ako_builtin_parityl(x)  __builtin_parityl(x)
#  define ako_builtin_parityll(x) __builtin_parityll(x)
#  define ako_builtin_parity32(x) AKO_BUILTIN__VARIANT_INT32(_,parity)(x)
#  define ako_builtin_parity64(x) AKO_BUILTIN__VARIANT_INT64(_,parity)(x)
#else
AKO_BUILTIN__PARITY_DEFINE_PORTABLE(parity, unsigned int)
AKO_BUILTIN__PARITY_DEFINE_PORTABLE(parityl, unsigned long)
AKO_BUILTIN__PARITY_DEFINE_PORTABLE(parityll, unsigned long long)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_parity(x)   ako_builtin_parity(x)
#    define __builtin_parityl(x)  ako_builtin_parityl(x)
#    define __builtin_parityll(x) ako_builtin_parityll(x)
#  endif
#endif

#if !defined(ako_builtin_parity32)
#  define ako_builtin_parity32(x) AKO_BUILTIN__VARIANT_INT32(ako,parity)(x)
#endif

#if !defined(ako_builtin_parity64)
#  define ako_builtin_parity64(x) AKO_BUILTIN__VARIANT_INT64(ako,parity)(x)
#endif

/*** __builtin_popcount ***/

#define AKO_BUILTIN__POPCOUNT_DEFINE_PORTABLE(f_n, T)	    \
  AKO_BUILTIN__FUNCTION				    \
  int ako_builtin_##f_n(T x) {                            \
    x = x - ((x >> 1) & (T)~(T)0/3);                        \
    x = (x & (T)~(T)0/15*3) + ((x >> 2) & (T)~(T)0/15*3);   \
    x = (x + (x >> 4)) & (T)~(T)0/255*15;                   \
    return (T)(x * ((T)~(T)0/255)) >> (sizeof(T) - 1) * 8;  \
  }

#if AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_popcount, 3, 4)
#  define ako_builtin_popcount(x)   __builtin_popcount(x)
#  define ako_builtin_popcountl(x)  __builtin_popcountl(x)
#  define ako_builtin_popcountll(x) __builtin_popcountll(x)
#  define ako_builtin_popcount32(x) AKO_BUILTIN__VARIANT_INT32(_,popcount)(x)
#  define ako_builtin_popcount64(x) AKO_BUILTIN__VARIANT_INT64(_,popcount)(x)
#else
AKO_BUILTIN__POPCOUNT_DEFINE_PORTABLE(popcount, unsigned int)
AKO_BUILTIN__POPCOUNT_DEFINE_PORTABLE(popcountl, unsigned long)
AKO_BUILTIN__POPCOUNT_DEFINE_PORTABLE(popcountll, unsigned long long)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_popcount(x)   ako_builtin_popcount(x)
#    define __builtin_popcountl(x)  ako_builtin_popcountl(x)
#    define __builtin_popcountll(x) ako_builtin_popcountll(x)
#  endif
#endif

#if !defined(ako_builtin_popcount32)
#  define ako_builtin_popcount32(x) AKO_BUILTIN__VARIANT_INT32(ako,popcount)(x)
#endif

#if !defined(ako_builtin_popcount64)
#  define ako_builtin_popcount64(x) AKO_BUILTIN__VARIANT_INT64(ako,popcount)(x)
#endif

/*** __builtin_clrsb ***/

#define AKO_BUILTIN__CLRSB_DEFINE_PORTABLE(f_n, clzfn, T) \
  AKO_BUILTIN__FUNCTION				    \
  int ako_builtin_##f_n(T x) {                            \
    return (AKO_BUILTIN_UNLIKELY(x == -1) ?		    \
	    ((int) sizeof(x) * 8) :			    \
	    ako_builtin_##clzfn((x < 0) ? ~x : x)) - 1;   \
  }

#if AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_clrsb, 4, 7)
#  define ako_builtin_clrsb(x)   __builtin_clrsb(x)
#  if !defined(__INTEL_COMPILER)
#    define ako_builtin_clrsbl(x)  __builtin_clrsbl(x)
#  else
#    if AKO_BUILTIN__SIZEOF_LONG == AKO_BUILTIN__SIZEOF_INT
#      define ako_builtin_clrsbl(x) ((long) __builtin_clrsb((int) x))
#    elif AKO_BUILTIN__SIZEOF_LONG == AKO_BUILTIN__SIZEOF_LLONG
#      define ako_builtin_clrsbl(x) ((long) __builtin_clrsbll((long long) x))
#    else
    AKO_BUILTIN__CLRSB_DEFINE_PORTABLE(clrsbl, clzl, long)
#    endif
#    if defined(AKO_BUILTIN_EMULATE_NATIVE)
#      define __builtin_clrsbl(x)  ako_builtin_clrsbl(x)
#    endif
#  endif
#  define ako_builtin_clrsbll(x) __builtin_clrsbll(x)
#  define ako_builtin_clrsb32(x) AKO_BUILTIN__VARIANT_INT32(_,clrsb)(x)
#  define ako_builtin_clrsb64(x) AKO_BUILTIN__VARIANT_INT64(_,clrsb)(x)
#else
    AKO_BUILTIN__CLRSB_DEFINE_PORTABLE(clrsb, clz, int)
    AKO_BUILTIN__CLRSB_DEFINE_PORTABLE(clrsbl, clzl, long)
    AKO_BUILTIN__CLRSB_DEFINE_PORTABLE(clrsbll, clzll, long long)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_clrsb(x)   ako_builtin_clrsb(x)
#    define __builtin_clrsbl(x)  ako_builtin_clrsbl(x)
#    define __builtin_clrsbll(x) ako_builtin_clrsbll(x)
#  endif
#endif

#if !defined(ako_builtin_clrsb32)
#  define ako_builtin_clrsb32(x) AKO_BUILTIN__VARIANT_INT32(ako,clrsb)(x)
#endif

#if !defined(ako_builtin_clrsb64)
#  define ako_builtin_clrsb64(x) AKO_BUILTIN__VARIANT_INT64(ako,clrsb)(x)
#endif

    /*** __builtin_addc ***/

#define AKO_BUILTIN__ADDC_DEFINE_PORTABLE(f_n, T)	\
  AKO_BUILTIN__FUNCTION				\
  T ako_builtin_##f_n(T x, T y, T ci, T* co) {	\
    T max = 0;						\
    T r = (T) x + y;					\
    max = ~max;						\
    *co = (T) (x > (max - y));				\
    if (ci) {						\
      if (r == max)					\
	*co = 1;					\
      r += ci;						\
    }							\
    return r;						\
  }

#if AKO_BUILTIN_CLANG_HAS_BUILTIN(__builtin_addc) && !defined(__ibmxl__)
#  define ako_builtin_addcb(x, y, ci, co)  __builtin_addcb(x, y, ci, co)
#  define ako_builtin_addcs(x, y, ci, co)  __builtin_addcs(x, y, ci, co)
#  define ako_builtin_addc(x, y, ci, co)   __builtin_addc(x, y, ci, co)
#  define ako_builtin_addcl(x, y, ci, co)  __builtin_addcl(x, y, ci, co)
#  define ako_builtin_addcll(x, y, ci, co) __builtin_addcll(x, y, ci, co)
#  define ako_builtin_addc8(x, y, ci, co)  AKO_BUILTIN__VARIANT2_INT8(_,addc)(x, y, ci, co)
#  define ako_builtin_addc16(x, y, ci, co) AKO_BUILTIN__VARIANT2_INT16(_,addc)(x, y, ci, co)
#  define ako_builtin_addc32(x, y, ci, co) AKO_BUILTIN__VARIANT2_INT32(_,addc)(x, y, ci, co)
#  define ako_builtin_addc64(x, y, ci, co) AKO_BUILTIN__VARIANT2_INT64(_,addc)(x, y, ci, co)
#else
    AKO_BUILTIN__ADDC_DEFINE_PORTABLE(addcb, unsigned char)
    AKO_BUILTIN__ADDC_DEFINE_PORTABLE(addcs, unsigned short)
    AKO_BUILTIN__ADDC_DEFINE_PORTABLE(addc, unsigned int)
    AKO_BUILTIN__ADDC_DEFINE_PORTABLE(addcl, unsigned long)
    AKO_BUILTIN__ADDC_DEFINE_PORTABLE(addcll, unsigned long long)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_addcb(x, y, ci, co)  ako_builtin_addcb(x, y, ci, co)
#    define __builtin_addcs(x, y, ci, co)  ako_builtin_addcs(x, y, ci, co)
#    define __builtin_addc(x, y, ci, co)   ako_builtin_addc(x, y, ci, co)
#    define __builtin_addcl(x, y, ci, co)  ako_builtin_addcl(x, y, ci, co)
#    define __builtin_addcll(x, y, ci, co) ako_builtin_addcll(x, y, ci, co)
#  endif
#endif

#if !defined(ako_builtin_addc8)
#  define ako_builtin_addc8(x, y, ci, co) \
     AKO_BUILTIN__VARIANT2_INT8(ako,addc)(x, y, ci, co)
#endif

#if !defined(ako_builtin_addc16)
#  define ako_builtin_addc16(x, y, ci, co) \
     AKO_BUILTIN__VARIANT2_INT16(ako,addc)(x, y, ci, co)
#endif

#if !defined(ako_builtin_addc32)
#  define ako_builtin_addc32(x, y, ci, co) \
     AKO_BUILTIN__VARIANT2_INT32(ako,addc)(x, y, ci, co)
#endif

#if !defined(ako_builtin_addc64)
#  define ako_builtin_addc64(x, y, ci, co) \
     AKO_BUILTIN__VARIANT2_INT64(ako,addc)(x, y, ci, co)
#endif

    /*** __builtin_subc ***/

#define AKO_BUILTIN__SUBC_DEFINE_PORTABLE(f_n, T)	\
  AKO_BUILTIN__FUNCTION				\
  T ako_builtin_##f_n(T x, T y, T ci, T* co) {	\
    T r = x - y;					\
    *co = x < y;					\
    if (ci) {						\
      r--;						\
      if (r == 0)					\
	*co = 1;					\
    }							\
    return r;						\
  }

#if AKO_BUILTIN_CLANG_HAS_BUILTIN(__builtin_subc) && !defined(__ibmxl__)
#  define ako_builtin_subcb(x, y, ci, co)  __builtin_subcb(x, y, ci, co)
#  define ako_builtin_subcs(x, y, ci, co)  __builtin_subcs(x, y, ci, co)
#  define ako_builtin_subc(x, y, ci, co)   __builtin_subc(x, y, ci, co)
#  define ako_builtin_subcl(x, y, ci, co)  __builtin_subcl(x, y, ci, co)
#  define ako_builtin_subcll(x, y, ci, co) __builtin_subcll(x, y, ci, co)
#  define ako_builtin_subc8(x, y, ci, co)  AKO_BUILTIN__VARIANT2_INT8(_,subc)(x, y, ci, co)
#  define ako_builtin_subc16(x, y, ci, co) AKO_BUILTIN__VARIANT2_INT16(_,subc)(x, y, ci, co)
#  define ako_builtin_subc32(x, y, ci, co) AKO_BUILTIN__VARIANT2_INT32(_,subc)(x, y, ci, co)
#  define ako_builtin_subc64(x, y, ci, co) AKO_BUILTIN__VARIANT2_INT64(_,subc)(x, y, ci, co)
#else
    AKO_BUILTIN__SUBC_DEFINE_PORTABLE(subcb, unsigned char)
    AKO_BUILTIN__SUBC_DEFINE_PORTABLE(subcs, unsigned short)
    AKO_BUILTIN__SUBC_DEFINE_PORTABLE(subc, unsigned int)
    AKO_BUILTIN__SUBC_DEFINE_PORTABLE(subcl, unsigned long)
    AKO_BUILTIN__SUBC_DEFINE_PORTABLE(subcll, unsigned long long)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_subcb(x, y, ci, co)  ako_builtin_subcb(x, y, ci, co)
#    define __builtin_subcs(x, y, ci, co)  ako_builtin_subcs(x, y, ci, co)
#    define __builtin_subc(x, y, ci, co)   ako_builtin_subc(x, y, ci, co)
#    define __builtin_subcl(x, y, ci, co)  ako_builtin_subcl(x, y, ci, co)
#    define __builtin_subcll(x, y, ci, co) ako_builtin_subcll(x, y, ci, co)
#  endif
#endif

#if !defined(ako_builtin_subc8)
#  define ako_builtin_subc8(x, y, ci, co) \
     AKO_BUILTIN__VARIANT2_INT8(ako,subc)(x, y, ci, co)
#endif

#if !defined(ako_builtin_subc16)
#  define ako_builtin_subc16(x, y, ci, co) \
     AKO_BUILTIN__VARIANT2_INT16(ako,subc)(x, y, ci, co)
#endif

#if !defined(ako_builtin_subc32)
#  define ako_builtin_subc32(x, y, ci, co) \
     AKO_BUILTIN__VARIANT2_INT32(ako,subc)(x, y, ci, co)
#endif

#if !defined(ako_builtin_subc64)
#  define ako_builtin_subc64(x, y, ci, co) \
     AKO_BUILTIN__VARIANT2_INT64(ako,subc)(x, y, ci, co)
#endif

    /*** __builtin_bswap ***/

#if AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_bswap16, 4, 8)
#  define ako_builtin_bswap16(x) __builtin_bswap16(x)
#else
#  if AKO_BUILTIN_MSVC_HAS_INTRIN(_byteswap_ushort,13,10)
#    define ako_builtin_bswap16(x) _byteswap_ushort(x)
#  else
    AKO_BUILTIN__FUNCTION
    ako_uint16_t
    ako_builtin_bswap16(ako_uint16_t v) {
    return
        ((v & (((ako_uint16_t)0xff) << 8)) >> 8) |
        ((v & (((ako_uint16_t)0xff))) << 8);
}
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_bswap16(x) ako_builtin_bswap16(x)
#  endif
#endif

#if AKO_BUILTIN_GNU_HAS_BUILTIN(__builtin_bswap16, 4, 3)
#  define ako_builtin_bswap32(x) __builtin_bswap32(x)
#  define ako_builtin_bswap64(x) __builtin_bswap64(x)
#else
#  if AKO_BUILTIN_MSVC_HAS_INTRIN(_byteswap_ushort,13,10)
#    define ako_builtin_bswap32(x) _byteswap_ulong(x)
#    define ako_builtin_bswap64(x) _byteswap_uint64(x)
#  else
AKO_BUILTIN__FUNCTION
ako_uint32_t
ako_builtin_bswap32(ako_uint32_t v) {
    return
        ((v & (((ako_uint32_t)0xff) << 24)) >> 24) |
        ((v & (((ako_uint32_t)0xff) << 16)) >> 8) |
        ((v & (((ako_uint32_t)0xff) << 8)) << 8) |
        ((v & (((ako_uint32_t)0xff))) << 24);
}

AKO_BUILTIN__FUNCTION
ako_uint64_t
ako_builtin_bswap64(ako_uint64_t v) {
    return
        ((v & (((ako_uint64_t)0xff) << 56)) >> 56) |
        ((v & (((ako_uint64_t)0xff) << 48)) >> 40) |
        ((v & (((ako_uint64_t)0xff) << 40)) >> 24) |
        ((v & (((ako_uint64_t)0xff) << 32)) >> 8) |
        ((v & (((ako_uint64_t)0xff) << 24)) << 8) |
        ((v & (((ako_uint64_t)0xff) << 16)) << 24) |
        ((v & (((ako_uint64_t)0xff) << 8)) << 40) |
        ((v & (((ako_uint64_t)0xff))) << 56);
}
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_bswap32(x) ako_builtin_bswap32(x)
#    define __builtin_bswap64(x) ako_builtin_bswap64(x)
#  endif
#endif

/*** __builtin_bitreverse ***/

#define AKO_BUILTIN__BITREVERSE_DEFINE_PORTABLE(f_n, b_l, T)	\
  AKO_BUILTIN__FUNCTION					\
  T ako_builtin_##f_n##b_l(T v) {				\
    v = ((v & ((T) 0xf0f0f0f0f0f0f0f0ULL)) >> 4) |		\
        ((v & ((T) 0x0f0f0f0f0f0f0f0fULL)) << 4);		\
    v = ((v & ((T) 0xccccccccccccccccULL)) >> 2) |		\
        ((v & ((T) 0x3333333333333333ULL)) << 2);		\
    v = ((v & ((T) 0xaaaaaaaaaaaaaaaaULL)) >> 1) |		\
        ((v & ((T) 0x5555555555555555ULL)) << 1);		\
    return ako_builtin_bswap##b_l(v);				\
  }

#if AKO_BUILTIN_CLANG_HAS_BUILTIN(__builtin_bitreverse64) && \
     !defined(__EMSCRIPTEN__) && !defined(__ibmxl__)
#  define ako_builtin_bitreverse8(x)  __builtin_bitreverse8(x)
#  define ako_builtin_bitreverse16(x) __builtin_bitreverse16(x)
#  define ako_builtin_bitreverse32(x) __builtin_bitreverse32(x)
#  define ako_builtin_bitreverse64(x) __builtin_bitreverse64(x)
#else
AKO_BUILTIN__FUNCTION
ako_uint8_t ako_builtin_bitreverse8(ako_uint8_t v) {
    return (ako_uint8_t)((v * 0x0202020202ULL & 0x010884422010ULL) % 1023);
}
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable:4310)
#endif
AKO_BUILTIN__BITREVERSE_DEFINE_PORTABLE(bitreverse, 16, ako_uint16_t)
AKO_BUILTIN__BITREVERSE_DEFINE_PORTABLE(bitreverse, 32, ako_uint32_t)
AKO_BUILTIN__BITREVERSE_DEFINE_PORTABLE(bitreverse, 64, ako_uint64_t)
#if defined(_MSC_VER)
#  pragma warning(pop)
#endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __builtin_bitreverse8(x)  ako_builtin_bitreverse8(x)
#    define __builtin_bitreverse16(x) ako_builtin_bitreverse16(x)
#    define __builtin_bitreverse32(x) ako_builtin_bitreverse32(x)
#    define __builtin_bitreverse64(x) ako_builtin_bitreverse64(x)
#  endif
#endif

/******
 *** MSVC-style intrinsics
 ******/

 /*** _rotl ***/

#define AKO_BUILTIN_ROTL_DEFINE_PORTABLE(f_n, T, ST)	\
  AKO_BUILTIN__FUNCTION                               \
  T ako_intrin_##f_n(T value, ST shift) {             \
    const ST mask = (ST) ((sizeof(T) * CHAR_BIT) - 1);  \
    return                                              \
      (value << (shift & mask)) |                       \
      (value >> (-shift & mask));                       \
  }

#if defined(AKO_BUILTIN__ENABLE_X86) && AKO_BUILTIN_MSVC_HAS_INTRIN(_rotl8, 14, 0)
#  define ako_intrin_rotl8(value, shift) _rotl8(value, shift)
#  define ako_intrin_rotl16(value, shift) _rotl16(value, shift)
#else
#  if defined(AKO_BUILTIN__ENABLE_X86) && AKO_BUILTIN_GNU_HAS_BUILTIN(__rolw, 4, 5)
#    define ako_intrin_rotl8(value, shift) __rolb(value, shift)
#    define ako_intrin_rotl16(value, shift) __rolw(value, shift)
#  elif defined(AKO_BUILTIN__ENABLE_X86) && !defined(_MSC_VER)
    AKO_BUILTIN__FUNCTION
    ako_uint32_t ako_intrin_rotl8(ako_uint8_t value, int shift) {
    __asm__("rolb %1, %0" : "+g" (value) : "cI" ((ako_uint8_t)shift));
    return value;
}

AKO_BUILTIN__FUNCTION
ako_uint32_t ako_intrin_rotl16(ako_uint16_t value, int shift) {
    __asm__("rolw %1, %0" : "+g" (value) : "cI" ((ako_uint8_t)shift));
    return value;
}
#  else
    AKO_BUILTIN_ROTL_DEFINE_PORTABLE(rotl8, ako_uint8_t, unsigned char)
    AKO_BUILTIN_ROTL_DEFINE_PORTABLE(rotl16, ako_uint16_t, unsigned char)
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _rotl8(value, shift)  ako_intrin_rotl8(value, shift)
#    define _rotl16(value, shift) ako_intrin_rotl16(value, shift)
#  endif
#endif

#if defined(AKO_BUILTIN__ENABLE_X86) && \
     (AKO_BUILTIN_MSVC_HAS_INTRIN(_rotl, 13, 10) || \
     AKO_BUILTIN_GNU_HAS_BUILTIN(_rotl, 4, 5) || \
     defined(__INTEL_COMPILER))
#  define ako_intrin_rotl(value, shift) _rotl(value, shift)
#else
#  if defined(AKO_BUILTIN__ENABLE_X86) && !defined(_MSC_VER)
    AKO_BUILTIN__FUNCTION
    ako_uint32_t ako_intrin_rotl(ako_uint32_t value, int shift) {
    __asm__("roll %1, %0" : "+g" (value) : "cI" ((ako_uint8_t)shift));
    return value;
}
#  else
    AKO_BUILTIN_ROTL_DEFINE_PORTABLE(rotl, ako_uint32_t, int)
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _rotl(value, shift) ako_intrin_rotl(value, shift)
#  endif
#endif

#if defined(AKO_BUILTIN__ENABLE_X86) && AKO_BUILTIN_MSVC_HAS_INTRIN(_rotl64, 13, 10)
#  define ako_intrin_rotl64(value, shift) _rotl64(value, shift)
#else
#  if defined(__amd64) && AKO_BUILTIN_GNU_HAS_BUILTIN(__rolq, 4, 5)
#    define ako_intrin_rotl64(value, shift) __rolq(value, shift)
#  elif defined(AKO_BUILTIN__ENABLE_X86) && !defined(_MSC_VER)
    AKO_BUILTIN__FUNCTION
    ako_uint64_t ako_intrin_rotl64(ako_uint64_t value, int shift) {
    __asm__("rolq %1, %0" : "+g" (value) : "cJ" ((ako_uint8_t)shift));
    return value;
}
#  else
    AKO_BUILTIN_ROTL_DEFINE_PORTABLE(rotl64, ako_uint64_t, int)
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _rotl64(value, shift) ako_intrin_rotl64(value, shift)
#  endif
#endif

    /*** _rotr ***/

#define AKO_BUILTIN_ROTR_DEFINE_PORTABLE(f_n, T, ST)	\
  AKO_BUILTIN__FUNCTION                               \
  T ako_intrin_##f_n(T value, ST shift) {             \
    const ST mask = (ST) ((sizeof(T) * CHAR_BIT) - 1);  \
    return                                              \
      (value >> (shift & mask)) |                       \
      (value << (-shift & mask));                       \
  }

#if defined(AKO_BUILTIN__ENABLE_X86) && AKO_BUILTIN_MSVC_HAS_INTRIN(_rotr8, 14, 0)
#  define ako_intrin_rotr8(value, shift) _rotr8(value, shift)
#  define ako_intrin_rotr16(value, shift) _rotr16(value, shift)
#else
#  if defined(AKO_BUILTIN__ENABLE_X86) && AKO_BUILTIN_GNU_HAS_BUILTIN(__rorw, 4, 5)
#    define ako_intrin_rotr8(value, shift) __rorb(value, shift)
#    define ako_intrin_rotr16(value, shift) __rorw(value, shift)
#  elif defined(AKO_BUILTIN__ENABLE_X86) && !defined(_MSC_VER)
    AKO_BUILTIN__FUNCTION
    ako_uint32_t ako_intrin_rotr8(ako_uint8_t value, int shift) {
    __asm__("rorb %1, %0" : "+g" (value) : "cI" ((ako_uint8_t)shift));
    return value;
}

AKO_BUILTIN__FUNCTION
ako_uint32_t ako_intrin_rotr16(ako_uint16_t value, int shift) {
    __asm__("rorw %1, %0" : "+g" (value) : "cI" ((ako_uint8_t)shift));
    return value;
}
#  else
    AKO_BUILTIN_ROTR_DEFINE_PORTABLE(rotr8, ako_uint8_t, unsigned char)
    AKO_BUILTIN_ROTR_DEFINE_PORTABLE(rotr16, ako_uint16_t, unsigned char)
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _rotr8(value, shift)  ako_intrin_rotr8(value, shift)
#    define _rotr16(value, shift) ako_intrin_rotr16(value, shift)
#  endif
#endif

#if defined(AKO_BUILTIN__ENABLE_X86) && \
     (AKO_BUILTIN_MSVC_HAS_INTRIN(_rotr, 13, 10) || \
     AKO_BUILTIN_GNU_HAS_BUILTIN(_rotr, 4, 5) || \
     defined(__INTEL_COMPILER))
#  define ako_intrin_rotr(value, shift) _rotr(value, shift)
#else
#  if defined(AKO_BUILTIN__ENABLE_X86) && !defined(_MSC_VER)
    AKO_BUILTIN__FUNCTION
    ako_uint32_t ako_intrin_rotr(ako_uint32_t value, int shift) {
    __asm__("rorl %1, %0" : "+g" (value) : "cI" ((ako_uint8_t)shift));
    return value;
}
#  elif defined(__ARM_ACLE) && (__ARM_ACLE >= 100)
#    define ako_intrin_rotr(value, shift) __ror(value, shift)
#  else
    AKO_BUILTIN_ROTR_DEFINE_PORTABLE(rotr, ako_uint32_t, int)
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _rotr(value, shift) ako_intrin_rotr(value, shift)
#  endif
#endif

#if defined(AKO_BUILTIN__ENABLE_X86) && AKO_BUILTIN_MSVC_HAS_INTRIN(_rotr64, 13, 10)
#  define ako_intrin_rotr64(value, shift) _rotr64(value, shift)
#else
#  if defined(__amd64) && AKO_BUILTIN_GNU_HAS_BUILTIN(__rorq, 4, 5)
#    define ako_intrin_rotr64(value, shift) __rorq(value, shift)
#  elif defined(AKO_BUILTIN__ENABLE_X86) && !defined(_MSC_VER)
    AKO_BUILTIN__FUNCTION
    ako_uint64_t ako_intrin_rotr64(ako_uint64_t value, int shift) {
    __asm__("rorq %1, %0" : "+g" (value) : "cJ" ((ako_uint8_t)shift));
    return value;
}
#  elif defined(__ARM_ACLE) && (__ARM_ACLE >= 110)
#    define ako_intrin_rotr64(value, shift) __rorll(value, shift)
#  else
    AKO_BUILTIN_ROTR_DEFINE_PORTABLE(rotr64, ako_uint64_t, int)
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _rotr64(value, shift) ako_intrin_rotr64(value, shift)
#  endif
#endif

    /*** _BitScanForward ***/

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_BitScanForward, 14, 0)
#  pragma intrinsic(_BitScanForward)
    AKO_BUILTIN__FUNCTION
    unsigned char ako_intrin_BitScanForward(unsigned long* Index, ako_uint32_t Mask) {
    const unsigned long M = (unsigned long)Mask;
    return _BitScanForward(Index, M);
}
#else
    AKO_BUILTIN__FUNCTION
    unsigned char ako_intrin_BitScanForward(unsigned long* Index, ako_uint32_t Mask) {
    return AKO_BUILTIN_UNLIKELY(Mask == 0) ? 0 : ((*Index = ako_builtin_ctz32(Mask)), 1);
}

#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _BitScanForward(Index, Mask) ako_intrin_BitScanForward(Index, Mask)
#  endif
#endif

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_BitScanForward64, 14, 0) && \
     (defined(_M_AMD64) || defined(_M_ARM))
#  pragma intrinsic(_BitScanForward64)
#  define ako_intrin_BitScanForward64(Index, Mask) _BitScanForward64(Index, Mask)
#else
AKO_BUILTIN__FUNCTION
unsigned char ako_intrin_BitScanForward64(unsigned long* Index, ako_uint64_t Mask) {
    return AKO_BUILTIN_UNLIKELY(Mask == 0) ? 0 : ((*Index = ako_builtin_ctz64(Mask)), 1);
}

#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _BitScanForward64(Index, Mask) ako_intrin_BitScanForward64(Index, Mask)
#  endif
#endif

/*** _BitScanReverse ***/

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_BitScanReverse, 14, 0)
#  pragma intrinsic(_BitScanReverse)
AKO_BUILTIN__FUNCTION
unsigned char ako_intrin_BitScanReverse(unsigned long* Index, ako_uint32_t Mask) {
    const unsigned long M = (unsigned long)Mask;
    return _BitScanReverse(Index, M);
}
#else
AKO_BUILTIN__FUNCTION
unsigned char ako_intrin_BitScanReverse(unsigned long* Index, ako_uint32_t Mask) {
    return (AKO_BUILTIN_UNLIKELY(Mask == 0)) ? 0 : \
        ((*Index = ((sizeof(Mask) * CHAR_BIT) - 1) - ako_builtin_clz32(Mask)), 1);
}

#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _BitScanReverse(Index, Mask) ako_intrin_BitScanReverse(Index, Mask)
#  endif
#endif

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_BitScanReverse64, 14, 0) && \
     (defined(_M_AMD64) || defined(_M_ARM))
#  pragma intrinsic(_BitScanReverse64)
#  define ako_intrin_BitScanReverse64(Index, Mask) _BitScanReverse64(Index, Mask)
#else
AKO_BUILTIN__FUNCTION
unsigned char ako_intrin_BitScanReverse64(unsigned long* Index, ako_uint64_t Mask) {
    return (AKO_BUILTIN_UNLIKELY(Mask == 0)) ? 0 : \
        ((*Index = ((sizeof(Mask) * CHAR_BIT) - 1) - ako_builtin_clz64(Mask)), 1);
}

#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _BitScanReverse64(Index, Mask) ako_intrin_BitScanReverse64(Index, Mask)
#  endif
#endif

/*** bittest ***/

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_bittest, 14, 0)
#  pragma intrinsic(_bittest)
#  define ako_intrin_bittest(a, b)		\
  __pragma(warning(push))			\
  __pragma(warning(disable:4057))		\
  _bittest(a, b)				\
  __pragma(warning(pop))
#else
#  define ako_intrin_bittest(a, b) (((*(a)) >> (b)) & 1)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _bittest(a, b) ako_intrin_bittest(a, b)
#  endif
#endif

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_bittest64, 14, 0) && \
     (defined(_M_AMD64) || defined(_M_ARM))
#  pragma intrinsic(_bittest64)
#  define ako_intrin_bittest64(a, b) _bittest64(a, b)
#else
#  define ako_intrin_bittest64(a, b) (((*(a)) >> (b)) & 1)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _bittest64(a, b) ako_intrin_bittest64(a, b)
#  endif
#endif

/*** bittestandcomplement ***/

#define AKO_BUILTIN__BITTESTANDCOMPLEMENT_DEFINE_PORTABLE(f_n, T, UT)	\
  AKO_BUILTIN__FUNCTION						\
  unsigned char ako_intrin_##f_n(T* a, T b) {				\
    const char r = (*a >> b) & 1;					\
    *a ^= ((UT) 1) << b;						\
    return r;								\
  }

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_bittestandcomplement, 14, 0)
#  pragma intrinsic(_bittestandcomplement)
#  define ako_intrin_bittestandcomplement(a, b)	\
  __pragma(warning(push))				\
  __pragma(warning(disable:4057))			\
  _bittestandcomplement(a, b)				\
  __pragma(warning(pop))
#else
AKO_BUILTIN__BITTESTANDCOMPLEMENT_DEFINE_PORTABLE(bittestandcomplement, \
    ako_int32_t, ako_uint32_t)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _bittestandcomplement(a, b) ako_intrin_bittestandcomplement(a, b)
#  endif
#endif

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_bittestandcomplement64, 14, 0) && defined(_M_AMD64)
#  define ako_intrin_bittestandcomplement64(a, b) _bittestandcomplement64(a, b)
#else
AKO_BUILTIN__BITTESTANDCOMPLEMENT_DEFINE_PORTABLE(bittestandcomplement64, \
    ako_int64_t, ako_uint64_t)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _bittestandcomplement64(a, b) ako_intrin_bittestandcomplement64(a, b)
#  endif
#endif

/*** bittestandreset ***/

#define AKO_BUILTIN__BITTESTANDRESET_DEFINE_PORTABLE(f_n, T, UT)	\
  AKO_BUILTIN__FUNCTION						\
  unsigned char ako_intrin_##f_n(T* a, T b) {				\
    const char r = (*a >> b) & 1;					\
    *a &= ~(((UT) 1) << b);						\
    return r;								\
  }

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_bittestandreset, 14, 0)
#  pragma intrinsic(_bittestandreset)
#  define ako_intrin_bittestandreset(a, b)	\
  __pragma(warning(push))			\
  __pragma(warning(disable:4057))		\
  _bittestandreset(a, b)			\
  __pragma(warning(pop))
#else
AKO_BUILTIN__BITTESTANDRESET_DEFINE_PORTABLE(bittestandreset, ako_int32_t, ako_uint32_t)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _bittestandreset(a, b) ako_intrin_bittestandreset(a, b)
#  endif
#endif

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_bittestandreset64, 14, 0) && \
     (defined(_M_AMD64) || defined(_M_IA64))
#  pragma intrinsic(_bittestandreset64)
#  define ako_intrin_bittestandreset64(a, b) _bittestandreset64(a, b)
#else
AKO_BUILTIN__BITTESTANDRESET_DEFINE_PORTABLE(bittestandreset64, ako_int64_t, ako_uint64_t)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _bittestandreset64(a, b) ako_intrin_bittestandreset64(a, b)
#  endif
#endif

/*** bittestandset ***/

#define AKO_BUILTIN__BITTESTANDSET_DEFINE_PORTABLE(f_n, T, UT)	\
  AKO_BUILTIN__FUNCTION						\
  unsigned char ako_intrin_##f_n(T* a, T b) {				\
    const char r = (*a >> b) & 1;					\
    *a |= ((UT) 1) << b;						\
    return r;								\
  }

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_bittestandset, 14, 0)
#  pragma intrinsic(_bittestandset)
#  define ako_intrin_bittestandset(a, b)	\
  __pragma(warning(push))			\
  __pragma(warning(disable:4057))		\
  _bittestandset(a, b)				\
  __pragma(warning(pop))
#else
AKO_BUILTIN__BITTESTANDSET_DEFINE_PORTABLE(bittestandset, ako_int32_t, ako_uint32_t)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _bittestandset(a, b) ako_intrin_bittestandset(a, b)
#  endif
#endif

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_bittestandset64, 14, 0) && defined(_M_AMD64)
#  pragma intrinsic(_bittestandset64)
#  define ako_intrin_bittestandset64(a, b) _bittestandset64(a, b)
#else
AKO_BUILTIN__BITTESTANDSET_DEFINE_PORTABLE(bittestandset64, ako_int64_t, ako_uint64_t)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _bittestandset64(a, b) ako_intrin_bittestandset64(a, b)
#  endif
#endif

/*** shiftleft128 ***/

#if AKO_BUILTIN_MSVC_HAS_INTRIN(__shiftleft128, 14, 0) && defined(_M_AMD64)
#  define ako_intrin_shiftleft128(LowPart, HighPart, Shift) __shiftleft128(LowPart, HighPart, Shift)
#else
#  if defined(__SIZEOF_INT128__)
AKO_BUILTIN__FUNCTION
ako_uint64_t ako_intrin_shiftleft128(ako_uint64_t LowPart, ako_uint64_t HighPart, \
    unsigned char Shift) {
    unsigned __int128 r = HighPart;
    r <<= 64;
    r |= LowPart;
    r <<= Shift % 64;
    return (ako_uint64_t)(r >> 64);
}
#  else
AKO_BUILTIN__FUNCTION
ako_uint64_t ako_intrin_shiftleft128(ako_uint64_t LowPart, ako_uint64_t HighPart, \
    unsigned char Shift) {
    Shift %= 64;
    return AKO_BUILTIN_UNLIKELY(Shift == 0) ? HighPart : \
        ((HighPart << Shift) | (LowPart >> (64 - Shift)));
}
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __shiftleft128(LowPart, HighPart, Shift) \
       ako_intrin_shiftleft128(LowPart, HighPart, Shift)
#  endif
#endif

/*** shiftright128 ***/

#if AKO_BUILTIN_MSVC_HAS_INTRIN(__shiftright128, 14, 0) && defined(_M_AMD64)
#  define ako_intrin_shiftright128(LowPart, HighPart, Shift) \
     __shiftright128(LowPart, HighPart, Shift)
#else
#  if defined(__SIZEOF_INT128__)
AKO_BUILTIN__FUNCTION
ako_uint64_t ako_intrin_shiftright128(ako_uint64_t LowPart, ako_uint64_t HighPart, \
    unsigned char Shift) {
    unsigned __int128 r = HighPart;
    r <<= 64;
    r |= LowPart;
    r >>= Shift % 64;
    return (ako_uint64_t)r;
}
#  else
AKO_BUILTIN__FUNCTION
ako_uint64_t ako_intrin_shiftright128(ako_uint64_t LowPart, ako_uint64_t HighPart, \
    unsigned char Shift) {
    Shift %= 64;

    if (AKO_BUILTIN_UNLIKELY(Shift == 0))
        return LowPart;

    return
        (HighPart << (64 - Shift)) |
        (LowPart >> Shift);
}
#  endif
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define __shiftright128(LowPart, HighPart, Shift) \
       ako_intrin_shiftright128(LowPart, HighPart, Shift)
#  endif
#endif

/*** byteswap ***/

#if AKO_BUILTIN_MSVC_HAS_INTRIN(_byteswap_ushort,13,10)
#  pragma intrinsic(_byteswap_ushort)
#  define ako_intrin_byteswap_ushort(v) _byteswap_ushort(v)
#  pragma intrinsic(_byteswap_ulong)
#  define ako_intrin_byteswap_ulong(v)  _byteswap_ulong(v)
#  pragma intrinsic(_byteswap_uint64)
#  define ako_intrin_byteswap_uint64(v) _byteswap_uint64(v)
#else
#  define ako_intrin_byteswap_ushort(v) ako_builtin_bswap16(v)
#  define ako_intrin_byteswap_ulong(v)  ako_builtin_bswap32(v)
#  define ako_intrin_byteswap_uint64(v) ako_builtin_bswap64(v)
#  if defined(AKO_BUILTIN_EMULATE_NATIVE)
#    define _byteswap_ushort(v) ako_intrin_byteswap_ushort(v)
#    define _byteswap_ulong(v)  ako_intrin_byteswap_ulong(v)
#    define _byteswap_uint64(v) ako_intrin_byteswap_uint64(v)
#  endif
#endif

#endif // AKOFS_AKO_BUILT_IN_H_
