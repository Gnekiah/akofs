#ifndef AKOFS_TEST_AKO_SLAB_HPP_
#define AKOFS_TEST_AKO_SLAB_HPP_

#include <gtest/gtest.h>
#include <ako_slab.h>
#include <ako_builtin.h>

#define AKO_SLOTS_ALL_ZERO      ((uint64_t) 0)
#define AKO_SLOTS_FIRST         ((uint64_t) 1)
#define AKO_FIRST_FREE_SLOT(s)  ((size_t) ako_builtin_ctzll(s))
#define AKO_FREE_SLOTS(s)       ((size_t) ako_builtin_popcount64(s))
#define AKO_ONE_USED_SLOT(slot, slot_mask) \
(((~(slot) & (slot_mask)) & ((~(slot) & (slot_mask)) - 1)) == AKO_SLOTS_ALL_ZERO)

#define AKO_POWER_OF2(x) ((x) != 0 && ((x) & ((x) - 1)) == 0)

/*
 * Slab Chain re-check
 */
static int __ako_slab_is_valid(const struct ako_slab_chain* const sch) {
    EXPECT_TRUE(AKO_POWER_OF2(ako_slab_pagesize));
    EXPECT_TRUE(AKO_POWER_OF2(sch->slabsize));
    EXPECT_TRUE(AKO_POWER_OF2(sch->pages_per_alloc));

    EXPECT_TRUE(sch->itemcount >= 2 && sch->itemcount <= 64);
    EXPECT_TRUE(sch->itemsize >= 1 && sch->itemsize <= SIZE_MAX);
    EXPECT_TRUE(sch->pages_per_alloc >= ako_slab_pagesize);
    EXPECT_TRUE(sch->pages_per_alloc >= sch->slabsize);

    EXPECT_TRUE(offsetof(struct ako_slab_header, data) +
        sch->itemsize * sch->itemcount <= sch->slabsize);

    EXPECT_TRUE(sch->empty_slotmask == ~AKO_SLOTS_ALL_ZERO >> (64 - sch->itemcount));
    EXPECT_TRUE(sch->initial_slotmask == (sch->empty_slotmask ^ AKO_SLOTS_FIRST));
    EXPECT_TRUE(sch->alignment_mask == ~(sch->slabsize - 1));

    const struct ako_slab_header* const heads[] = {
        sch->full,
        sch->empty,
        sch->partial
    };

    for (size_t head = 0; head < 3; ++head) {
        const struct ako_slab_header* prev = NULL;
        const struct ako_slab_header* slab = NULL;

        for (slab = heads[head]; slab != NULL; slab = slab->next) {
            EXPECT_TRUE((slab->prev == (prev == NULL ? NULL : prev)));

            switch (head) {
            case 0:
                EXPECT_TRUE(slab->slots == AKO_SLOTS_ALL_ZERO);
                break;
            case 1:
                EXPECT_TRUE(slab->slots == sch->empty_slotmask);
                break;
            case 2:
                EXPECT_TRUE((slab->slots & ~sch->empty_slotmask) == AKO_SLOTS_ALL_ZERO);
                EXPECT_TRUE(AKO_FREE_SLOTS(slab->slots) >= 1);
                EXPECT_TRUE(AKO_FREE_SLOTS(slab->slots) < sch->itemcount);
                break;
            }

            if (slab->refcount == 0) {
                EXPECT_EQ(0, (uintptr_t)slab % sch->slabsize);

                if (sch->slabsize >= ako_slab_pagesize)
                    EXPECT_TRUE((uintptr_t)slab->page % sch->slabsize == 0);
                else
                    EXPECT_TRUE((uintptr_t)slab->page % ako_slab_pagesize == 0);
            }
            else {
                if (sch->slabsize >= ako_slab_pagesize)
                    EXPECT_TRUE((uintptr_t)slab % sch->slabsize == 0);
                else
                    EXPECT_TRUE((uintptr_t)slab % ako_slab_pagesize == 0);
            }
            prev = slab;
        }
    }
    return 1;
}

/*
 * Slab Chain dump
 */
static void __ako_slab_dump(const struct ako_slab_chain* const sch, char* str, int size) {
    EXPECT_TRUE(sch);
    EXPECT_TRUE(__ako_slab_is_valid(sch));

    int ret = 0;
    uint64_t total_nr_slabs = 0, total_used_slots = 0, total_AKO_FREE_SLOTS = 0;
    float occupancy = 0;
    const struct ako_slab_header* const heads[] = {
        sch->partial,
        sch->empty,
        sch->full
    };
    const char* labels[] = {
        "Partial",
        "Empty",
        "Full"
    };

    ret = snprintf(str, size, "%18s: %8zu\n"
        "%18s: %8zu = %.2f * (%zu pagesize)\n"
        "%18s: %8zu = (%zu offset) + (%zu itemcount) * (%zu itemsize)\n"
        "%18s: %8zu = (%zu slabsize) - (%zu total)\n"
        "%18s: %8zu = %zu * (%zu pagesize)\n"
        "%18s: %8zu = (%zu alloc) / (%zu slabsize)\n\n",
        "pagesize",
        ako_slab_pagesize,
        "slabsize",
        sch->slabsize, (float)sch->slabsize / ako_slab_pagesize, ako_slab_pagesize,
        "total",
        offsetof(struct ako_slab_header, data) + sch->itemcount * sch->itemsize,
        offsetof(struct ako_slab_header, data), sch->itemcount, sch->itemsize,
        "waste per slab",
        sch->slabsize - offsetof(struct ako_slab_header, data) -
        sch->itemcount * sch->itemsize, sch->slabsize,
        offsetof(struct ako_slab_header, data) + sch->itemcount * sch->itemsize,
        "pages per alloc",
        sch->pages_per_alloc, sch->pages_per_alloc / ako_slab_pagesize,
        ako_slab_pagesize,
        "slabs per alloc",
        sch->pages_per_alloc / sch->slabsize, sch->pages_per_alloc,
        sch->slabsize
    );

    str += ret;
    size -= ret;

    ret = snprintf(str, size, "%8s %17s %17s %17s %17s\n", "",
        "Slabs", "Used", "Free", "Occupancy");

    for (size_t i = 0; i < 3; ++i) {
        uint64_t nr_slabs = 0, used_slots = 0, AKO_FREE_SLOTS = 0;
        const struct ako_slab_header* slab;

        for (slab = heads[i]; slab != NULL; slab = slab->next) {
            nr_slabs++;
            used_slots += sch->itemcount - AKO_FREE_SLOTS(slab->slots);
            AKO_FREE_SLOTS += AKO_FREE_SLOTS(slab->slots);
        }

        occupancy = used_slots + AKO_FREE_SLOTS ?
            100 * (float)used_slots / (used_slots + AKO_FREE_SLOTS) : 0.0;

        str += ret;
        size -= ret;

        ret = snprintf(str, size, "%8s %17llu %17llu %17llu %16.2f%%\n",
            labels[i], nr_slabs, used_slots, AKO_FREE_SLOTS, occupancy);

        total_nr_slabs += nr_slabs;
        total_used_slots += used_slots;
        total_AKO_FREE_SLOTS += AKO_FREE_SLOTS;
    }

    occupancy = total_used_slots + total_AKO_FREE_SLOTS ?
        100 * (float)total_used_slots / (total_used_slots + total_AKO_FREE_SLOTS) : 0.0;

    str += ret;
    size -= ret;

    ret = snprintf(str, size, "%8s %17llu %17llu %17llu %16.2f%%\n\n", "Total",
        total_nr_slabs, total_used_slots, total_AKO_FREE_SLOTS, occupancy);

    for (size_t i = 0; i < 3; ++i) {
        const struct ako_slab_header* slab = heads[i];
        str += ret;
        size -= ret;

        ret = snprintf(str, size, "%6s |%2d%13s|%2d%13s|%2d%13s|%2d%13s\n",
            labels[i], 64, "", 48, "", 32, "", 16, "");

        uint64_t total = 0, row;

        for (row = 1; slab != NULL; slab = slab->next, ++row) {
            const unsigned used = sch->itemcount - AKO_FREE_SLOTS(slab->slots);
            str += ret;
            size -= ret;

            ret = snprintf(str, size, "%6llu ", row);

            for (int k = 63; k >= 0; --k) {
                str += ret;
                size -= ret;

                ret = snprintf(str, size, slab->slots & (AKO_SLOTS_FIRST << k) ? "1" :
                    ((size_t)k >= sch->itemcount ? "0" : "0"));
            }

            str += ret;
            size -= ret;

            ret = snprintf(str, size, " %8u\n", used);
            total += used;
        }

        str += ret;
        size -= ret;

        ret = snprintf(str, size, "%6s ^%15s^%15s^%15s^%15s %8llu\n\n",
            "", "", "", "", "", total);
    }
}

TEST(test_ako_slab, case_create_slab_and_usage_test_1) {
    struct ako_slab_chain s;
    double* allocs[16 * 60];
    int buff_size = 1024 * 16;
    char* buff = (char*)malloc(buff_size);
    
    ako_slab_init(&s, sizeof(double));

    for (size_t i = 0; i < sizeof(allocs) / sizeof(*allocs); i++) {
        allocs[i] = (double*)ako_slab_alloc(&s);
        EXPECT_TRUE(allocs[i] != NULL);
        *allocs[i] = i * 4;
        __ako_slab_dump(&s, buff, buff_size);
        //std::cerr << buff << std::endl;
    }

    for (int64_t i = sizeof(allocs) / sizeof(*allocs) - 1; i >= 0; i--) {
        ako_slab_free(&s, allocs[i]);
        __ako_slab_dump(&s, buff, buff_size);
        //std::cerr << buff << std::endl;
    }

    ako_slab_destroy(&s);
}

#endif // AKOFS_TEST_AKO_SLAB_HPP_
