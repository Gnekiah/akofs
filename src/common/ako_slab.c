/*
 * slab allocator in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <ako_slab.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <ako_builtin.h>

#ifdef __linux__
#include <unistd.h>
#include <sys/mman.h>

#else
#define _SC_PAGESIZE                0
#define PROT_READ                   0
#define PROT_WRITE                  0
#define MAP_PRIVATE                 0
#define MAP_ANONYMOUS               0
#define MAP_FAILED                  NULL
#define AKO_SLAB_PAGE_SIZE_DEFAULT  4096
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

#define AKO_SLOTS_ALL_ZERO      ((uint64_t) 0)
#define AKO_SLOTS_FIRST         ((uint64_t) 1)
#define AKO_FIRST_FREE_SLOT(s)  ((size_t) ako_builtin_ctzll(s))
#define AKO_FREE_SLOTS(s)       ((size_t) ako_builtin_popcount64(s))

#define AKO_ONE_USED_SLOT(slot, slot_mask) \
(((~(slot) & (slot_mask)) & ((~(slot) & (slot_mask)) - 1)) == AKO_SLOTS_ALL_ZERO)

#define AKO_POWER_OF2(x) ((x) != 0 && ((x) & ((x) - 1)) == 0)

size_t ako_slab_pagesize;


AKO_UNUSED static int __ako_slab_is_valid(const struct ako_slab_chain* const sch) {
#ifndef AKO_DEBUG_OFF
    assert(AKO_POWER_OF2(ako_slab_pagesize));
    assert(AKO_POWER_OF2(sch->slabsize));
    assert(AKO_POWER_OF2(sch->pages_per_alloc));

    assert(sch->itemcount >= 2 && sch->itemcount <= 64);
    assert(sch->itemsize >= 1 && sch->itemsize <= SIZE_MAX);
    assert(sch->pages_per_alloc >= ako_slab_pagesize);
    assert(sch->pages_per_alloc >= sch->slabsize);

    assert(offsetof(struct ako_slab_header, data) +
        sch->itemsize * sch->itemcount <= sch->slabsize);

    assert(sch->empty_slotmask == ~AKO_SLOTS_ALL_ZERO >> (64 - sch->itemcount));
    assert(sch->initial_slotmask == (sch->empty_slotmask ^ AKO_SLOTS_FIRST));
    assert(sch->alignment_mask == ~(sch->slabsize - 1));

    const struct ako_slab_header* const heads[] = {
        sch->full, 
        sch->empty, 
        sch->partial 
    };

    for (size_t head = 0; head < 3; ++head) {
        const struct ako_slab_header* prev = NULL;
        const struct ako_slab_header* slab = NULL;

        for (slab = heads[head]; slab != NULL; slab = slab->next) {
            assert((slab->prev == (prev == NULL ? NULL : prev)));

            switch (head) {
            case 0:
                assert(slab->slots == AKO_SLOTS_ALL_ZERO);
                break;
            case 1:
                assert(slab->slots == sch->empty_slotmask);
                break;
            case 2:
                assert((slab->slots & ~sch->empty_slotmask) == AKO_SLOTS_ALL_ZERO);
                assert(AKO_FREE_SLOTS(slab->slots) >= 1);
                assert(AKO_FREE_SLOTS(slab->slots) < sch->itemcount);
                break;
            }

            if (slab->refcount == 0) {
                assert((uintptr_t)slab % sch->slabsize == 0);

                if (sch->slabsize >= ako_slab_pagesize)
                    assert((uintptr_t)slab->page % sch->slabsize == 0);
                else
                    assert((uintptr_t)slab->page % ako_slab_pagesize == 0);
            }
            else {
                if (sch->slabsize >= ako_slab_pagesize)
                    assert((uintptr_t)slab % sch->slabsize == 0);
                else
                    assert((uintptr_t)slab % ako_slab_pagesize == 0);
            }
            prev = slab;
        }
    }
#endif
    return 1;
}

void ako_slab_init(struct ako_slab_chain* const sch, const size_t itemsize) {
    ako_slab_pagesize = (size_t)sysconf(_SC_PAGESIZE);

    assert(sch != NULL);
    assert(itemsize >= 1 && itemsize <= SIZE_MAX);
    assert(AKO_POWER_OF2(ako_slab_pagesize));

    sch->itemsize = itemsize;

    const size_t data_offset = offsetof(struct ako_slab_header, data);
    const size_t least_slabsize = data_offset + 64 * sch->itemsize;
    sch->slabsize = (size_t)1 << (size_t)ceil(log2((double)least_slabsize));
    sch->itemcount = 64;

    if (sch->slabsize - least_slabsize != 0) {
        const size_t shrinked_slabsize = sch->slabsize >> 1;

        if (data_offset < shrinked_slabsize &&
            shrinked_slabsize - data_offset >= 2 * sch->itemsize) {

            sch->slabsize = shrinked_slabsize;
            sch->itemcount = (shrinked_slabsize - data_offset) / sch->itemsize;
        }
    }

    sch->pages_per_alloc = sch->slabsize > ako_slab_pagesize ?
        sch->slabsize : ako_slab_pagesize;

    sch->empty_slotmask = ~AKO_SLOTS_ALL_ZERO >> (64 - sch->itemcount);
    sch->initial_slotmask = sch->empty_slotmask ^ AKO_SLOTS_FIRST;
    sch->alignment_mask = ~(sch->slabsize - 1);
    sch->partial = sch->empty = sch->full = NULL;

    assert(__ako_slab_is_valid(sch));
}

void* ako_slab_alloc(struct ako_slab_chain* const sch) {
    assert(sch != NULL);
    assert(__ako_slab_is_valid(sch));

    if (likely(sch->partial != NULL)) {
        /* found a partial slab, locate the first free slot */
        register const size_t slot = 
            AKO_FIRST_FREE_SLOT((unsigned long)sch->partial->slots);
        sch->partial->slots ^= AKO_SLOTS_FIRST << slot;

        if (unlikely(sch->partial->slots == AKO_SLOTS_ALL_ZERO)) {
            /* slab has become full, change state from partial to full */
            struct ako_slab_header* const tmp = sch->partial;

            /* skip first slab from partial list */
            if (likely((sch->partial = sch->partial->next) != NULL))
                sch->partial->prev = NULL;

            if (likely((tmp->next = sch->full) != NULL))
                sch->full->prev = tmp;

            sch->full = tmp;
            return sch->full->data + slot * sch->itemsize;
        }
        else {
            return sch->partial->data + slot * sch->itemsize;
        }
    }
    else if (likely((sch->partial = sch->empty) != NULL)) {
        /* found an empty slab, change state from empty to partial */
        if (likely((sch->empty = sch->empty->next) != NULL))
            sch->empty->prev = NULL;

        sch->partial->next = NULL;

        /* slab is located either at the beginning of page, or beyond */
        unlikely(sch->partial->refcount != 0) ?
            sch->partial->refcount++ : sch->partial->page->refcount++;

        sch->partial->slots = sch->initial_slotmask;
        return sch->partial->data;
    }
    else {
        /* no empty or partial slabs available, create a new one */
        if (sch->slabsize <= ako_slab_pagesize) {
            sch->partial = mmap(NULL, sch->pages_per_alloc,
                PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

            if (unlikely(sch->partial == MAP_FAILED)) {
                sch->partial = NULL;
                return NULL;
            }
        }
        else {
            const int err = posix_memalign((void**)& sch->partial,
                sch->slabsize, sch->pages_per_alloc);

            if (unlikely(err != 0)) {
                //fprintf(stderr, "posix_memalign(align=%zu, size=%zu): %d\n",
                //    sch->slabsize, sch->pages_per_alloc, err);

                return sch->partial = NULL;
            }
        }

        struct ako_slab_header* prev = NULL;

        const char* const page_end = (char*)sch->partial + sch->pages_per_alloc;

        union {
            const char* c;
            struct ako_slab_header* const s;
        } curr = {
            .c = (const char*)sch->partial + sch->slabsize,
        };

#ifdef __GNUC__
        __builtin_prefetch(sch->partial, 1);
#endif

        sch->partial->prev = sch->partial->next = NULL;
        sch->partial->refcount = 1;
        sch->partial->slots = sch->initial_slotmask;

        if (likely(curr.c != page_end)) {
            curr.s->prev = NULL;
            curr.s->refcount = 0;
            curr.s->page = sch->partial;
            curr.s->slots = sch->empty_slotmask;
            sch->empty = prev = curr.s;

            while (likely((curr.c += sch->slabsize) != page_end)) {
                prev->next = curr.s;
                curr.s->prev = prev;
                curr.s->refcount = 0;
                curr.s->page = sch->partial;
                curr.s->slots = sch->empty_slotmask;
                prev = curr.s;
            }
            prev->next = NULL;
        }
        return sch->partial->data;
    }
    /* unreachable */
    return NULL;
}

void ako_slab_free(struct ako_slab_chain* const sch, const void* const addr) {
    assert(sch != NULL);
    assert(addr != NULL);
    assert(__ako_slab_is_valid(sch));

    int err = 0;
    struct ako_slab_header* const slab = (void*)((uintptr_t)addr & sch->alignment_mask);

    register const int slot = ((char*)addr - (char*)slab -
        offsetof(struct ako_slab_header, data)) / sch->itemsize;

    if (unlikely(slab->slots == AKO_SLOTS_ALL_ZERO)) {
        /* target slab is full, change state to partial */
        slab->slots = AKO_SLOTS_FIRST << slot;

        if (likely(slab != sch->full)) {
            if (likely(slab->prev && (slab->prev->next = slab->next) != NULL))
                slab->next->prev = slab->prev;

            slab->prev = NULL;
        }
        else if (likely((sch->full = sch->full->next) != NULL)) {
            sch->full->prev = NULL;
        }

        slab->next = sch->partial;

        if (likely(sch->partial != NULL))
            sch->partial->prev = slab;

        sch->partial = slab;
    }
    else if (unlikely(AKO_ONE_USED_SLOT(slab->slots, sch->empty_slotmask))) {
        /* target slab is partial and has only one filled slot */
        if (unlikely(slab->refcount == 1 || (slab->refcount == 0 &&
            slab->page->refcount == 1))) {

            /* unmap the whole page if this slab is the only partial one */
            if (likely(slab != sch->partial)) {
                if (likely((slab->prev->next = slab->next) != NULL))
                    slab->next->prev = slab->prev;
            }
            else if (likely((sch->partial = sch->partial->next) != NULL)) {
                sch->partial->prev = NULL;
            }

            void* const page = unlikely(slab->refcount != 0) ? slab : slab->page;
            const char* const page_end = (char*)page + sch->pages_per_alloc;
            char found_head = 0;

            union {
                const char* c;
                const struct ako_slab_header* const s;
            } s;

            for (s.c = page; s.c != page_end; s.c += sch->slabsize) {
                if (unlikely(s.s == sch->empty))
                    found_head = 1;
                else if (unlikely(s.s == slab))
                    continue;
                else if (likely((s.s->prev->next = s.s->next) != NULL))
                    s.s->next->prev = s.s->prev;
            }

            if (unlikely(found_head && (sch->empty = sch->empty->next) != NULL))
                sch->empty->prev = NULL;

            if (sch->slabsize <= ako_slab_pagesize) {
                err = munmap(page, sch->pages_per_alloc);
                if (unlikely(err == -1))
                    //perror("munmap");
                    ;
            }
            else {
                free(page);
            }
        }
        else {
            slab->slots = sch->empty_slotmask;

            if (likely(slab != sch->partial)) {
                if (likely((slab->prev->next = slab->next) != NULL))
                    slab->next->prev = slab->prev;

                slab->prev = NULL;
            }
            else if (likely((sch->partial = sch->partial->next) != NULL)) {
                sch->partial->prev = NULL;
            }

            slab->next = sch->empty;

            if (likely(sch->empty != NULL))
                sch->empty->prev = slab;

            sch->empty = slab;

            unlikely(slab->refcount != 0) ? slab->refcount-- : slab->page->refcount--;
        }
    }
    else {
        /* target slab is partial, no need to change state */
        slab->slots |= AKO_SLOTS_FIRST << slot;
    }
}

void ako_slab_traverse(const struct ako_slab_chain* const sch, void (*fn)(const void*)) {
    assert(fn != NULL);
    assert(sch != NULL);
    assert(__ako_slab_is_valid(sch));

    const struct ako_slab_header* slab;
    const char* item, * end;
    const size_t data_offset = offsetof(struct ako_slab_header, data);

    for (slab = sch->partial; slab; slab = slab->next) {
        item = (const char*)slab + data_offset;
        end = item + sch->itemcount * sch->itemsize;
        uint64_t mask = AKO_SLOTS_FIRST;

        do {
            if (!(slab->slots & mask))
                fn(item);

            mask <<= 1;
        } while ((item += sch->itemsize) != end);
    }

    for (slab = sch->full; slab; slab = slab->next) {
        item = (const char*)slab + data_offset;
        end = item + sch->itemcount * sch->itemsize;

        do fn(item);
        while ((item += sch->itemsize) != end);
    }
}

void ako_slab_destroy(const struct ako_slab_chain* const sch) {
    assert(sch != NULL);
    assert(__ako_slab_is_valid(sch));

    struct ako_slab_header* const heads[] = { 
        sch->partial, 
        sch->empty, 
        sch->full 
    };
    struct ako_slab_header* pages_head = NULL;
    struct ako_slab_header* pages_tail = NULL;

    for (size_t i = 0; i < 3; ++i) {
        struct ako_slab_header* slab = heads[i];

        while (slab != NULL) {
            if (slab->refcount != 0) {
                struct ako_slab_header* const page = slab;
                slab = slab->next;

                if (unlikely(pages_head == NULL))
                    pages_head = page;
                else
                    pages_tail->next = page;

                pages_tail = page;
            }
            else {
                slab = slab->next;
            }
        }
    }

    if (likely(pages_head != NULL)) {
        pages_tail->next = NULL;
        struct ako_slab_header* page = pages_head;

        if (sch->slabsize <= ako_slab_pagesize) {
            do {
                void* const target = page;
                page = page->next;

                if (unlikely(munmap(target, sch->pages_per_alloc) == -1))
                    //perror("munmap");
                    ;
            } while (page != NULL);
        }
        else {
            do {
                void* const target = page;
                page = page->next;
                free(target);
            } while (page != NULL);
        }
    }
}

