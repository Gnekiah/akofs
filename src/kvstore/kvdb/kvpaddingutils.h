//
//  kvpaddingutils.h
//  kvdb
//
//  Created by DINH Viêt Hoà on 6/2/13.
//  Copyright (c) 2013 etpan. All rights reserved.
//

#ifndef KVPADDINGUTILS_H
#define KVPADDINGUTILS_H

#if defined(__linux__)
#include <sys/mman.h>
#else

#ifndef TYPEDEF_SSIZE_T
#define TYPEDEF_SSIZE_T
typedef signed long long ssize_t;
#endif

#define _SC_PAGESIZE                0
#define PROT_READ                   0
#define PROT_WRITE                  0
#define MAP_PRIVATE                 0
#define MAP_ANONYMOUS               0
#define MAP_FAILED                  NULL
#define SPK_SLAB_PAGE_SIZE_DEFAULT  4096
#define sysconf(x)                  (4096)

static int __check_align(size_t align) {
    for (size_t i = sizeof(void*); i != 0; i *= 2)
        if (align == i)
            return 0;
    return EINVAL;
}

static int posix_memalign(void** ptr, size_t align, size_t size) {
    if (__check_align(align))
        return EINVAL;
    void* p = _aligned_malloc(size, align);
    if (p == NULL)
        return ENOMEM;
    *ptr = p;
    return 0;
}

static void* mmap(void* start, size_t length, int prot, int flags, int fd, int64_t offset) {
    if (__check_align(length))
        return NULL;
    return _aligned_malloc(length, length);
}

static int munmap(void* start, size_t length) {
    _aligned_free(start);
    return 0;
}
#endif

static inline uint64_t power2_round_up(uint64_t value)
{
    uint64_t power = 1;
    while (power < value)
        power <<= 1;
    return power;
}

static inline uint64_t block_size_round_up(uint64_t value)
{
    if (value < 16) {
        value = 16;
    }
    return power2_round_up(value);
}

static inline unsigned int log2_round_up(uint64_t value)
{
    uint64_t power = 1;
    unsigned int log2_value = 0;
    while (power < value) {
        power <<= 1;
        log2_value ++;
    }
    return log2_value;
}

#define KV_ULONG_PTR unsigned long
#define KV_PAGE_ROUND_UP(db, x) ( (((KV_ULONG_PTR)(x)) + db->kv_pagesize-1)  & (~(db->kv_pagesize-1)) )
#define KV_PAGE_ROUND_DOWN(db, x) ( ((KV_ULONG_PTR)(x)) & (~(db->kv_pagesize-1)) )
#define KV_BYTE_ROUND_UP(x) ( (((KV_ULONG_PTR)(x)) + 8-1)  & (~(8-1)) )

#endif
