/*
 * slab allocator in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_AKO_SLAB_H_
#define AKOFS_AKO_SLAB_H_

#ifndef __GNUC__
//#warning (Sparkle slab allocator can noly be used on POSIX Plat.)
// Support MSVC now ~  (^_^)    - 2019/12/08 12:03:56
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <ako_compat.h>

extern size_t ako_slab_pagesize;

typedef struct ako_slab_header {
    struct ako_slab_header* prev;
    struct ako_slab_header*next;
    uint64_t slots;
    uintptr_t refcount;
    struct ako_slab_header* page;
    AKO_ALIGNED(uint8_t data[], 8);
} ako_slab_header;

typedef struct ako_slab_chain {
    size_t itemsize;
    size_t itemcount;
    size_t slabsize;
    size_t pages_per_alloc;
    uint64_t initial_slotmask;
    uint64_t empty_slotmask;
    uintptr_t alignment_mask;
    struct ako_slab_header* partial;
    struct ako_slab_header* empty;
    struct ako_slab_header* full;
} ako_slab_chain;

extern void ako_slab_init(struct ako_slab_chain* const, const size_t);
extern void* ako_slab_alloc(struct ako_slab_chain* const);
extern void ako_slab_free(struct ako_slab_chain* const, const void* const);
extern void ako_slab_traverse(const struct ako_slab_chain* const, void (*)(const void*));
extern void ako_slab_destroy(const struct ako_slab_chain* const);

#ifdef __cplusplus
}
#endif

#endif // AKOFS_AKO_SLAB_H_
