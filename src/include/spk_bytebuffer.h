/*
 * byte_buffer in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_BYTE_BUFFER_H_
#define SPARKLE_SPK_BYTE_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <spk_compat.h>

typedef SPK_PACKED(struct spk_byte_buffer_t {
    uint64_t pos;   // Read/Write position
    size_t len;     // Length of valid data array
    size_t size;    // Size of total buffer array, size=0 if wrapped
    uint8_t* buf;   // buffer ptr
    bool wrapped;   // True if this byte buffer is a wrapping buf
}) spk_byte_buffer;

// Memory allocation functions
extern spk_byte_buffer* spk_bytebuf_new_wrap(uint8_t* buf, size_t len);
extern spk_byte_buffer* spk_bytebuf_new_copy(const uint8_t* buf, size_t len);
extern spk_byte_buffer* spk_bytebuf_new(size_t len);
extern spk_byte_buffer* spk_bytebuf_new_default();
extern spk_byte_buffer* spk_bytebuf_resize(spk_byte_buffer* bbuf, size_t new_len);
extern spk_byte_buffer* spk_bytebuf_resize_force(spk_byte_buffer* bbuf, size_t new_len);
extern void spk_bytebuf_free(spk_byte_buffer* bbuf);

// Utility
extern void spk_bytebuf_reset(spk_byte_buffer* bbuf);
extern void spk_bytebuf_skip(spk_byte_buffer* bbuf, size_t len);
extern size_t spk_bytebuf_length(spk_byte_buffer* bbuf);
extern size_t spk_bytebuf_size(spk_byte_buffer* bbuf);
extern size_t spk_bytebuf_pos(spk_byte_buffer* bbuf);
extern bool spk_bytebuf_is_empty(spk_byte_buffer* bbuf);
extern bool spk_bytebuf_is_full(spk_byte_buffer* bbuf);
extern size_t spk_bytebuf_valid(spk_byte_buffer* bbuf);
extern bool spk_bytebuf_clear(spk_byte_buffer* bbuf);
extern spk_byte_buffer* spk_bytebuf_clone(spk_byte_buffer* bbuf);
extern bool spk_bytebuf_equals(spk_byte_buffer* bbuf1, spk_byte_buffer* bbuf2);
extern void spk_bytebuf_replace(spk_byte_buffer* bbuf, uint8_t key, uint8_t rep, 
    uint32_t start, bool first_occurance_only);
extern void spk_bytebuf_print_ascii(spk_byte_buffer* bbuf, char* dst);
extern void spk_bytebuf_print_hex(spk_byte_buffer* bbuf, char* dst);

// Read functions
extern uint8_t spk_bytebuf_peek(spk_byte_buffer* bbuf);
extern uint8_t spk_bytebuf_get(spk_byte_buffer* bbuf);
extern uint8_t spk_bytebuf_get_at(spk_byte_buffer* bbuf, uint64_t index);
extern void spk_bytebuf_get_bytes_in(spk_byte_buffer* bbuf, uint8_t* dest, size_t len);
extern void spk_bytebuf_get_bytes_at_in(spk_byte_buffer* bbuf, uint64_t index, 
    uint8_t* dest, size_t len);
extern uint8_t* spk_bytebuf_get_bytes(spk_byte_buffer* bbuf, size_t len);
extern uint8_t* spk_bytebuf_get_bytes_at(spk_byte_buffer* bbuf, size_t len, uint64_t index);
extern double spk_bytebuf_get_double(spk_byte_buffer* bbuf);
extern double spk_bytebuf_get_double_at(spk_byte_buffer* bbuf, uint64_t index);
extern float spk_bytebuf_get_float(spk_byte_buffer* bbuf);
extern float spk_bytebuf_get_float_at(spk_byte_buffer* bbuf, uint64_t index);
extern uint32_t spk_bytebuf_get_int(spk_byte_buffer* bbuf);
extern uint32_t spk_bytebuf_get_int_at(spk_byte_buffer* bbuf, uint64_t index);
extern uint64_t spk_bytebuf_get_long(spk_byte_buffer* bbuf);
extern uint64_t spk_bytebuf_get_long_at(spk_byte_buffer* bbuf, uint64_t index);
extern uint16_t spk_bytebuf_get_short(spk_byte_buffer* bbuf);
extern uint16_t spk_bytebuf_get_short_at(spk_byte_buffer* bbuf, uint64_t index);

// Put functions (simply drop bytes until there is no more room)
extern void spk_bytebuf_put_bytebuf(spk_byte_buffer* dest, spk_byte_buffer* src);
extern void spk_bytebuf_put(spk_byte_buffer* bbuf, uint8_t value);
extern void spk_bytebuf_put_at(spk_byte_buffer* bbuf, uint8_t value, uint64_t index);
extern void spk_bytebuf_put_bytes(spk_byte_buffer* bbuf, uint8_t* arr, size_t len);
extern void spk_bytebuf_put_bytes_at(spk_byte_buffer* bbuf, uint8_t* arr, size_t len, 
    uint64_t index);
extern void spk_bytebuf_put_double(spk_byte_buffer* bbuf, double value);
extern void spk_bytebuf_put_double_at(spk_byte_buffer* bbuf, double value, uint64_t index);
extern void spk_bytebuf_put_float(spk_byte_buffer* bbuf, float value);
extern void spk_bytebuf_put_float_at(spk_byte_buffer* bbuf, float value, uint64_t index);
extern void spk_bytebuf_put_int(spk_byte_buffer* bbuf, uint32_t value);
extern void spk_bytebuf_put_int_at(spk_byte_buffer* bbuf, uint32_t value, uint64_t index);
extern void spk_bytebuf_put_long(spk_byte_buffer* bbuf, uint64_t value);
extern void spk_bytebuf_put_long_at(spk_byte_buffer* bbuf, uint64_t value, uint64_t index);
extern void spk_bytebuf_put_short(spk_byte_buffer* bbuf, uint16_t value);
extern void spk_bytebuf_put_short_at(spk_byte_buffer* bbuf, uint16_t value, uint64_t index);

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_BYTE_BUFFER_H_