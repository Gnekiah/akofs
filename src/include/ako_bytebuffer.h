/*
 * byte_buffer in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_AKO_BYTE_BUFFER_H_
#define AKOFS_AKO_BYTE_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <ako_compat.h>

typedef AKO_PACKED(struct ako_byte_buffer_t {
    uint64_t pos;   // Read/Write position
    size_t len;     // Length of valid data array
    size_t size;    // Size of total buffer array, size=0 if wrapped
    uint8_t* buf;   // buffer ptr
    bool wrapped;   // True if this byte buffer is a wrapping buf
}) ako_byte_buffer;

// Memory allocation functions
extern ako_byte_buffer* ako_bytebuf_new_wrap(uint8_t* buf, size_t len);
extern ako_byte_buffer* ako_bytebuf_new_copy(const uint8_t* buf, size_t len);
extern ako_byte_buffer* ako_bytebuf_new(size_t len);
extern ako_byte_buffer* ako_bytebuf_new_default();
extern ako_byte_buffer* ako_bytebuf_resize(ako_byte_buffer* bbuf, size_t new_len);
extern ako_byte_buffer* ako_bytebuf_resize_force(ako_byte_buffer* bbuf, size_t new_len);
extern void ako_bytebuf_free(ako_byte_buffer* bbuf);

// Utility
extern void ako_bytebuf_reset(ako_byte_buffer* bbuf);
extern void ako_bytebuf_skip(ako_byte_buffer* bbuf, size_t len);
extern size_t ako_bytebuf_length(ako_byte_buffer* bbuf);
extern size_t ako_bytebuf_size(ako_byte_buffer* bbuf);
extern size_t ako_bytebuf_pos(ako_byte_buffer* bbuf);
extern bool ako_bytebuf_is_empty(ako_byte_buffer* bbuf);
extern bool ako_bytebuf_is_full(ako_byte_buffer* bbuf);
extern size_t ako_bytebuf_valid(ako_byte_buffer* bbuf);
extern bool ako_bytebuf_clear(ako_byte_buffer* bbuf);
extern ako_byte_buffer* ako_bytebuf_clone(ako_byte_buffer* bbuf);
extern bool ako_bytebuf_equals(ako_byte_buffer* bbuf1, ako_byte_buffer* bbuf2);
extern void ako_bytebuf_replace(ako_byte_buffer* bbuf, uint8_t key, uint8_t rep, 
    uint32_t start, bool first_occurance_only);
extern void ako_bytebuf_print_ascii(ako_byte_buffer* bbuf, char* dst);
extern void ako_bytebuf_print_hex(ako_byte_buffer* bbuf, char* dst);

// Read functions
extern uint8_t ako_bytebuf_peek(ako_byte_buffer* bbuf);
extern uint8_t ako_bytebuf_get(ako_byte_buffer* bbuf);
extern uint8_t ako_bytebuf_get_at(ako_byte_buffer* bbuf, uint64_t index);
extern void ako_bytebuf_get_bytes_in(ako_byte_buffer* bbuf, uint8_t* dest, size_t len);
extern void ako_bytebuf_get_bytes_at_in(ako_byte_buffer* bbuf, uint64_t index, 
    uint8_t* dest, size_t len);
extern uint8_t* ako_bytebuf_get_bytes(ako_byte_buffer* bbuf, size_t len);
extern uint8_t* ako_bytebuf_get_bytes_at(ako_byte_buffer* bbuf, size_t len, uint64_t index);
extern double ako_bytebuf_get_double(ako_byte_buffer* bbuf);
extern double ako_bytebuf_get_double_at(ako_byte_buffer* bbuf, uint64_t index);
extern float ako_bytebuf_get_float(ako_byte_buffer* bbuf);
extern float ako_bytebuf_get_float_at(ako_byte_buffer* bbuf, uint64_t index);
extern uint32_t ako_bytebuf_get_int(ako_byte_buffer* bbuf);
extern uint32_t ako_bytebuf_get_int_at(ako_byte_buffer* bbuf, uint64_t index);
extern uint64_t ako_bytebuf_get_long(ako_byte_buffer* bbuf);
extern uint64_t ako_bytebuf_get_long_at(ako_byte_buffer* bbuf, uint64_t index);
extern uint16_t ako_bytebuf_get_short(ako_byte_buffer* bbuf);
extern uint16_t ako_bytebuf_get_short_at(ako_byte_buffer* bbuf, uint64_t index);

// Put functions (simply drop bytes until there is no more room)
extern void ako_bytebuf_put_bytebuf(ako_byte_buffer* dest, ako_byte_buffer* src);
extern void ako_bytebuf_put(ako_byte_buffer* bbuf, uint8_t value);
extern void ako_bytebuf_put_at(ako_byte_buffer* bbuf, uint8_t value, uint64_t index);
extern void ako_bytebuf_put_bytes(ako_byte_buffer* bbuf, uint8_t* arr, size_t len);
extern void ako_bytebuf_put_bytes_at(ako_byte_buffer* bbuf, uint8_t* arr, size_t len, 
    uint64_t index);
extern void ako_bytebuf_put_double(ako_byte_buffer* bbuf, double value);
extern void ako_bytebuf_put_double_at(ako_byte_buffer* bbuf, double value, uint64_t index);
extern void ako_bytebuf_put_float(ako_byte_buffer* bbuf, float value);
extern void ako_bytebuf_put_float_at(ako_byte_buffer* bbuf, float value, uint64_t index);
extern void ako_bytebuf_put_int(ako_byte_buffer* bbuf, uint32_t value);
extern void ako_bytebuf_put_int_at(ako_byte_buffer* bbuf, uint32_t value, uint64_t index);
extern void ako_bytebuf_put_long(ako_byte_buffer* bbuf, uint64_t value);
extern void ako_bytebuf_put_long_at(ako_byte_buffer* bbuf, uint64_t value, uint64_t index);
extern void ako_bytebuf_put_short(ako_byte_buffer* bbuf, uint16_t value);
extern void ako_bytebuf_put_short_at(ako_byte_buffer* bbuf, uint16_t value, uint64_t index);

#ifdef __cplusplus
}
#endif

#endif // AKOFS_AKO_BYTE_BUFFER_H_