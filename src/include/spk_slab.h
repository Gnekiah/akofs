/*
 * slab allocator in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_SLAB_H_
#define SPARKLE_SPK_SLAB_H_

#ifndef __GNUC__
//#warning (Sparkle slab allocator can noly be used on POSIX Plat.)
// Support MSVC now ~  (^_^)    - 2019/12/08 12:03:56
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <spk_compat.h>

extern size_t spk_slab_pagesize;

typedef struct spk_slab_header {
    struct spk_slab_header* prev;
    struct spk_slab_header*next;
    uint64_t slots;
    uintptr_t refcount;
    struct spk_slab_header* page;
    SPK_ALIGNED(uint8_t data[], 8);
} spk_slab_header;

typedef struct spk_slab_chain {
    size_t itemsize;
    size_t itemcount;
    size_t slabsize;
    size_t pages_per_alloc;
    uint64_t initial_slotmask;
    uint64_t empty_slotmask;
    uintptr_t alignment_mask;
    struct spk_slab_header* partial;
    struct spk_slab_header* empty;
    struct spk_slab_header* full;
} spk_slab_chain;

extern void spk_slab_init(struct spk_slab_chain* const, const size_t);
extern void* spk_slab_alloc(struct spk_slab_chain* const);
extern void spk_slab_free(struct spk_slab_chain* const, const void* const);
extern void spk_slab_traverse(const struct spk_slab_chain* const, void (*)(const void*));
extern void spk_slab_destroy(const struct spk_slab_chain* const);

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_SLAB_H_
