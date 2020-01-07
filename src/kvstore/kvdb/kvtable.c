//
//  table.c
//  kvdb
//
//  Created by DINH Viêt Hoà on 6/2/13.
//  Copyright (c) 2013 etpan. All rights reserved.
//

#include "kvtable.h"
#include <stdio.h>

#if defined(__linux__)
#include <strings.h>
#include <unistd.h>
#include <sys/mman.h>
#else

#ifndef TYPEDEF_SSIZE_T
#define TYPEDEF_SSIZE_T
typedef signed long long ssize_t;
#endif

#include <errno.h>
#define _SC_PAGESIZE                0
#define PROT_READ                   0
#define PROT_WRITE                  0
#define MAP_SHARED                  0
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

#include "kvtypes.h"
#include "kvprime.h"
#include "kvpaddingutils.h"

static int map_table(kvdb * db, struct kvdb_table ** result, uint64_t offset, int is_first);
static int mapping_setup(struct kvdb_mapping * mapping, int fd, off_t offset, size_t size);
static void mapping_unsetup(struct kvdb_mapping * mapping);
static void unmap_table(struct kvdb_table * table);

int kv_table_header_write(kvdb * db, uint64_t table_start, uint64_t maxcount)
{
    uint64_t bloomsize = kv_getnextprime(maxcount * KV_TABLE_BITS_FOR_BLOOM_FILTER);
    char data[KV_TABLE_HEADER_SIZE];
    bzero(data, KV_TABLE_HEADER_SIZE);
    h64_to_bytes(&data[KV_TABLE_BLOOM_SIZE_OFFSET], bloomsize);
    h64_to_bytes(&data[KV_TABLE_MAX_COUNT_OFFSET], maxcount);
    ssize_t r;
    r = pwrite(db->kv_fd, data, KV_TABLE_HEADER_SIZE, table_start);
    if (r < 0)
        return -1;
    return 0;
}

int kv_tables_setup(kvdb * db)
{
    map_table(db, &db->kv_first_table, KV_HEADER_SIZE, 1);
    return 0;
}

void kv_tables_unsetup(kvdb * db)
{
    unmap_table(db->kv_first_table);
}

uint64_t kv_table_create(kvdb * db, uint64_t size, struct kvdb_table ** result)
{
    //fprintf(stderr, "create table %llu", (unsigned long long) size);
    uint64_t mapping_size = KV_TABLE_SIZE(size);
    uint64_t offset = ntoh64(* db->kv_filesize);
    int r;
    r = ftruncate(db->kv_fd, offset + mapping_size);
    if (r < 0)
        return 0;
    uint64_t filesize = ntoh64(* db->kv_filesize);
    filesize += mapping_size;
    r = kv_table_header_write(db, offset, size);
    if (r < 0)
        return 0;
    r = map_table(db, result, offset, 0);
    if (r < 0)
        return 0;
    
    // When everything succeeded, update file size
    * db->kv_filesize = hton64(filesize);
    
    return offset;
}

static int map_table(kvdb * db, struct kvdb_table ** result, uint64_t offset, int is_first)
{
    struct kvdb_table * table;
    uint64_t maxcount;
    ssize_t read_result;
    char data[8];
    int r;
    off_t pre_page_align_size;
    
    table = calloc(1, sizeof(* table));
    if (is_first) {
        pre_page_align_size = KV_HEADER_SIZE;
    }
    else {
        off_t mapping_offset = KV_PAGE_ROUND_DOWN(db, offset);
        pre_page_align_size = offset - mapping_offset;
    }
    
    read_result = pread(db->kv_fd, data, 8, offset + KV_TABLE_MAX_COUNT_OFFSET);
    if (read_result < 0) {
        return -1;
    }
    maxcount = bytes_to_h64(data);
    uint64_t mapping_size = pre_page_align_size + KV_TABLE_SIZE(maxcount);
    r = mapping_setup(&table->kv_mapping, db->kv_fd, offset - pre_page_align_size, (size_t) mapping_size);
    if (r < 0) {
        return -1;
    }
    table->kv_table_start = table->kv_mapping.kv_bytes + pre_page_align_size;
    
    table->kv_items = (struct kvdb_item *) (table->kv_table_start + KV_TABLE_ITEMS_OFFSET_OFFSET(maxcount));
    table->kv_next_table_offset = (uint64_t *) (table->kv_table_start + KV_TABLE_NEXT_TABLE_OFFSET_OFFSET);
    table->kv_count = (uint64_t *) (table->kv_table_start + KV_TABLE_COUNT_OFFSET);
    table->kv_bloom_filter_size = (uint64_t *) (table->kv_table_start + KV_TABLE_BLOOM_SIZE_OFFSET);
    table->kv_maxcount = (uint64_t *) (table->kv_table_start + KV_TABLE_MAX_COUNT_OFFSET);
    table->kv_bloom_filter = (uint8_t *) (table->kv_table_start + KV_TABLE_BLOOM_FILTER_OFFSET);
    
    * result = table;
    
    if (* table->kv_next_table_offset != 0) {
        r = map_table(db, &table->kv_next_table, ntoh64(* table->kv_next_table_offset), 0);
        if (r < 0) {
            return -1;
        }
    }
    else {
        table->kv_next_table = NULL;
    }
    
    return 0;
}

static void unmap_table(struct kvdb_table * table)
{
    if (table == NULL)
        return;
    
    struct kvdb_table * next_table = table->kv_next_table;
    mapping_unsetup(&table->kv_mapping);
    free(table);
    
    unmap_table(next_table);
}

static int mapping_setup(struct kvdb_mapping * mapping, int fd, off_t offset, size_t size)
{
    mapping->kv_bytes = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    if (mapping->kv_bytes == MAP_FAILED) {
        return -1;
    }
    mapping->kv_size = size;
    
    return 0;
}

static void mapping_unsetup(struct kvdb_mapping * mapping)
{
    if (mapping->kv_bytes == NULL) {
        return;
    }
    
    int r;
    r = munmap(mapping->kv_bytes, mapping->kv_size);
    if (r < 0) {
        fprintf(stderr, "Could not unmap memory\n");
    }
    mapping->kv_bytes = NULL;
    mapping->kv_size = 0;
}
