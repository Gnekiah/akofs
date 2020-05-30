/*
 * byte_buffer Implementation in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <ako_bytebuffer.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Default number of bytes to allocate in the backing buffer if no size is provided
static const uint64_t ako_byte_buffer_default_size = 4096;

// Wrap around an existing buf - will not copy buf
ako_byte_buffer* ako_bytebuf_new_wrap(uint8_t* buf, size_t len) {
    ako_byte_buffer* bbuf = (ako_byte_buffer*)malloc(sizeof(ako_byte_buffer));
    if (!bbuf)
        return NULL;

    bbuf->pos = 0;
    bbuf->wrapped = true;
    bbuf->len = len;
    bbuf->size = len;
    bbuf->buf = buf;
    return bbuf;
}

// Copy len bytes from buf into the newly created byte buffer
ako_byte_buffer* ako_bytebuf_new_copy(const uint8_t* buf, size_t len) {
    ako_byte_buffer* bbuf = (ako_byte_buffer*)malloc(sizeof(ako_byte_buffer));
    if (!bbuf)
        return NULL;

    bbuf->pos = 0;
    bbuf->wrapped = false;
    bbuf->len = len;
    bbuf->size = len;
    bbuf->buf = (uint8_t*)malloc(len);
    if (!bbuf->buf) {
        free(bbuf);
        return NULL;
    }
        
    memcpy(bbuf->buf, buf, len);
    return bbuf;
}

ako_byte_buffer* ako_bytebuf_new(size_t len) {
    ako_byte_buffer* bbuf = (ako_byte_buffer*)malloc(sizeof(ako_byte_buffer));
    if (!bbuf)
        return NULL;

    bbuf->pos = 0;
    bbuf->wrapped = false;
    bbuf->len = 0;
    bbuf->size = len;
    bbuf->buf = (uint8_t*)malloc(len);
    if (!bbuf->buf) {
        free(bbuf);
        return NULL;
    }

    memset(bbuf->buf, 0, len);
    return bbuf;
}

ako_byte_buffer* ako_bytebuf_new_default() {
    ako_byte_buffer* bbuf = (ako_byte_buffer*)malloc(sizeof(ako_byte_buffer));
    if (!bbuf)
        return NULL;

    bbuf->pos = 0;
    bbuf->wrapped = false;
    bbuf->len = 0;
    bbuf->size = ako_byte_buffer_default_size;
    bbuf->buf = (uint8_t*)malloc(ako_byte_buffer_default_size);
    if (!bbuf->buf) {
        free(bbuf);
        return NULL;
    }

    memset(bbuf->buf, 0, ako_byte_buffer_default_size);
    return bbuf;
}

/*
 * Resizes the internal buffer and copys of the new_len worth of data from the old buffer.
 * Resizing will only work on buffers managed by byte_buffer (ie. not wrapped buffers)
 * This will also reset the read/write position.
 *
 * @return ako_byte_buffer* if the resize was a success
 */
ako_byte_buffer* ako_bytebuf_resize(ako_byte_buffer* bbuf, size_t new_len) {
    // Can't resize an internal buffer that may be used elsewhere
    if (bbuf->wrapped)
        return NULL;

    // Resizing will only work on the internal buffer data is lesser than new_len.
    if (new_len < bbuf->len)
        return NULL;

    // Copy as much data from the old buffer as we can
    uint8_t* old_buf = bbuf->buf;
    bbuf->pos = 0;
    bbuf->size = new_len;
    bbuf->buf = (uint8_t*)malloc(new_len);
    if (!bbuf->buf) {
        bbuf->buf = old_buf;
        return NULL;
    }

    memcpy(bbuf->buf, old_buf, bbuf->len);
    free(old_buf);

    return bbuf;
}

/*
 * Resizes the internal buffer and copys of the new_len worth of data from the old buffer.
 * Resizing will only work on buffers managed by byte_buffer (ie. not wrapped buffers)
 * This will also reset the read/write position.
 *
 * WARNING: this function will force to cover valid buffer data.
 *
 * @return ako_byte_buffer* if the resize was a success
 */
ako_byte_buffer* ako_bytebuf_resize_force(ako_byte_buffer* bbuf, size_t new_len) {
    // Can't resize an internal buffer that may be used elsewhere
    if (bbuf->wrapped)
        return NULL;

    // Copy as much data from the old buffer as we can
    uint8_t* old_buf = bbuf->buf;
    bbuf->pos = 0;
    bbuf->size = new_len;
    bbuf->buf = (uint8_t*)malloc(new_len);
    if (!bbuf->buf) {
        bbuf->buf = old_buf;
        return NULL;
    }

    size_t copy_len = new_len < bbuf->len ? new_len : bbuf->len;
    memcpy(bbuf->buf, old_buf, copy_len);
    bbuf->len = copy_len;
    free(old_buf);

    return bbuf;
}

inline void ako_bytebuf_free(ako_byte_buffer* bbuf) {
    if (!bbuf->wrapped)
        free(bbuf->buf);

    free(bbuf);
}

/****************************************************/
//////////////////////////////////////////////////////
/****************************************************/

inline void ako_bytebuf_reset(ako_byte_buffer* bbuf) {
    bbuf->pos = 0;
}

inline void ako_bytebuf_skip(ako_byte_buffer* bbuf, size_t len) {
    size_t pos = bbuf->pos + len;
    bbuf->pos = pos > bbuf->len ? bbuf->len : pos;
}

inline size_t ako_bytebuf_length(ako_byte_buffer* bbuf) {
    return bbuf->len;
}

inline size_t ako_bytebuf_size(ako_byte_buffer* bbuf) {
    return bbuf->size;
}

inline size_t ako_bytebuf_pos(ako_byte_buffer* bbuf) {
    return bbuf->pos;
}

inline bool ako_bytebuf_is_empty(ako_byte_buffer* bbuf) {
    if (bbuf->wrapped)
        return false;
    return bbuf->len == 0;
}

inline bool ako_bytebuf_is_full(ako_byte_buffer* bbuf) {
    if (bbuf->wrapped)
        return true;
    return bbuf->len == bbuf->size;
}

// Number of bytes from the current read position till the end of the buffer
inline size_t ako_bytebuf_valid(ako_byte_buffer* bbuf) {
    return bbuf->size - bbuf->len;
}

// Blank out the buffer and reset the position
inline bool ako_bytebuf_clear(ako_byte_buffer* bbuf) {
    if (bbuf->wrapped)
        return false;
    memset(bbuf->buf, 0, bbuf->size);
    bbuf->pos = 0;
    bbuf->len = 0;
    return true;
}

// Return a new instance of a bytebuffer with the exact same contents and the same state
inline ako_byte_buffer* ako_bytebuf_clone(ako_byte_buffer* bbuf) {
    ako_byte_buffer* ret = ako_bytebuf_new_copy(bbuf->buf, bbuf->len);
    if (ret)
        ret->pos = bbuf->pos;
    return ret;
}

// Compare if the contents are equivalent
inline bool ako_bytebuf_equals(ako_byte_buffer* bbuf1, ako_byte_buffer* bbuf2) {
    if (bbuf1->len != bbuf2->len)
        return false;

    for (uint64_t i = 0; i < bbuf1->len; i++) {
        if (bbuf1->buf[i] != bbuf2->buf[i])
            return false;
    }

    return true;
}

void ako_bytebuf_replace(ako_byte_buffer* bbuf, uint8_t key, uint8_t rep, uint32_t start,
    bool first_occurance_only) {
    for (uint64_t i = start; i < bbuf->len; i++) {
        if (bbuf->buf[i] == key) {
            bbuf->buf[i] = rep;

            if (first_occurance_only)
                return;
        }
    }
}

inline void ako_bytebuf_print_ascii(ako_byte_buffer* bbuf, char* dst) {
    uint64_t i = 0;
    for (i = 0; i < bbuf->len; i++) {
        dst[i] = bbuf->buf[i];
    }
    dst[i] = 0;
}

inline void ako_bytebuf_print_hex(ako_byte_buffer* bbuf, char* dst) {
    uint64_t i = 0, j = 0;
    for (i = 0, j = 0; i < bbuf->len; i++) {
        uint8_t temp1 = (bbuf->buf[i] & 0xf0) >> 4;
        uint8_t temp2 = bbuf->buf[i] & 0x0f;
        dst[j++] = temp1 > 9 ? 'a' + temp1 - 10 : '0' + temp1;
        dst[j++] = temp2 > 9 ? 'a' + temp2 - 10 : '0' + temp2;
    }
    dst[j] = 0;
}

// Relative peek. Reads and returns the next byte in the buffer from the current
// position but does not increment the read position
inline uint8_t ako_bytebuf_peek(ako_byte_buffer* bbuf) {
    //return *(uint8_t*)(bb->buf+bb->pos);
    //size_t pos = bbuf->pos == 0 ? 0 : bbuf->pos - 1;
    return bbuf->buf[bbuf->pos];
}

// Relative get method. Reads the byte at the buffers current position then 
// increments the position
inline uint8_t ako_bytebuf_get(ako_byte_buffer* bbuf) {
    return bbuf->pos == bbuf->len ? 0 : bbuf->buf[bbuf->pos++];
}

// Absolute get method. Read byte at index
inline uint8_t ako_bytebuf_get_at(ako_byte_buffer* bbuf, uint64_t index) {
    return bbuf->buf[index];
}

inline void ako_bytebuf_get_bytes_in(ako_byte_buffer* bbuf, uint8_t* dest, size_t len) {
    for (size_t i = 0; i < len; i++) {
        dest[i] = ako_bytebuf_get(bbuf);
    }
}

inline void ako_bytebuf_get_bytes_at_in(ako_byte_buffer* bbuf, uint64_t index,
    uint8_t* dest, size_t len) {
    for (size_t i = 0; i < len; i++) {
        dest[i] = ako_bytebuf_get_at(bbuf, index + i);
    }
}

// Return a new byte array of size len with the contents from the current position
uint8_t* ako_bytebuf_get_bytes(ako_byte_buffer* bbuf, size_t len) {
    uint8_t* ret = (uint8_t*)malloc(len);
    if (!ret)
        return NULL;
    size_t s = MIN(len, bbuf->len - bbuf->pos);
    memcpy(ret, bbuf->buf + bbuf->pos, s);
    bbuf->pos += s;
    return ret;
}

// Return a new byte array of size len with the contents from the index position
uint8_t* ako_bytebuf_get_bytes_at(ako_byte_buffer* bbuf, size_t len, uint64_t index) {
    uint8_t* ret = (uint8_t*)malloc(len);
    if (!ret)
        return NULL;
    memcpy(ret, bbuf->buf + index, len);
    return ret;
}

double ako_bytebuf_get_double(ako_byte_buffer* bbuf) {
    if (bbuf->pos + sizeof(double) > bbuf->len)
        return 0;
    double ret = *(double*)(bbuf->buf + bbuf->pos);
    bbuf->pos += sizeof(double);
    return ret;
}

inline double ako_bytebuf_get_double_at(ako_byte_buffer* bbuf, uint64_t index) {
    return *(double*)(bbuf->buf + index);
}

float ako_bytebuf_get_float(ako_byte_buffer* bbuf) {
    if (bbuf->pos + sizeof(float) > bbuf->len)
        return 0;
    float ret = *(float*)(bbuf->buf + bbuf->pos);
    bbuf->pos += sizeof(float);
    return ret;
}

inline float ako_bytebuf_get_float_at(ako_byte_buffer* bbuf, uint64_t index) {
    return *(float*)(bbuf->buf + index);
}

uint32_t ako_bytebuf_get_int(ako_byte_buffer* bbuf) {
    if (bbuf->pos + sizeof(uint32_t) > bbuf->len)
        return 0;
    uint32_t ret = *(uint32_t*)(bbuf->buf + bbuf->pos);
    bbuf->pos += sizeof(uint32_t);
    return ret;
}

inline uint32_t ako_bytebuf_get_int_at(ako_byte_buffer* bbuf, uint64_t index) {
    return *(uint32_t*)(bbuf->buf + index);
}

uint64_t ako_bytebuf_get_long(ako_byte_buffer* bbuf) {
    if (bbuf->pos + sizeof(uint64_t) > bbuf->len)
        return 0;
    uint64_t ret = *(uint64_t*)(bbuf->buf + bbuf->pos);
    bbuf->pos += sizeof(uint64_t);
    return ret;
}

inline uint64_t ako_bytebuf_get_long_at(ako_byte_buffer* bbuf, uint64_t index) {
    return *(uint64_t*)(bbuf->buf + index);
}

uint16_t ako_bytebuf_get_short(ako_byte_buffer* bbuf) {
    if (bbuf->pos + sizeof(uint16_t) > bbuf->len)
        return 0;
    uint16_t ret = *(uint16_t*)(bbuf->buf + bbuf->pos);
    bbuf->pos += sizeof(uint16_t);
    return ret;
}

inline uint16_t ako_bytebuf_get_short_at(ako_byte_buffer* bbuf, uint64_t index) {
    return *(uint16_t*)(bbuf->buf + index);
}

// Relative write of the entire contents of another ByteBuffer (src)
void ako_bytebuf_put_bytebuf(ako_byte_buffer* dest, ako_byte_buffer* src) {
    size_t i = 0;
    while (i < src->len && i < dest->size) {
        ako_bytebuf_put(dest, src->buf[i]);
        i++;
    }
}

void ako_bytebuf_put(ako_byte_buffer* bbuf, uint8_t value) {
    if (bbuf->len >= bbuf->size)
        return;
    bbuf->buf[bbuf->len++] = value;
}

void ako_bytebuf_put_at(ako_byte_buffer* bbuf, uint8_t value, uint64_t index) {
    if (index >= bbuf->len)
        return;

    bbuf->buf[index] = value;
}

void ako_bytebuf_put_bytes(ako_byte_buffer* bbuf, uint8_t* arr, size_t len) {
    for (size_t i = 0; i < len; i++) {
        ako_bytebuf_put(bbuf, arr[i]);
    }
}

void ako_bytebuf_put_bytes_at(ako_byte_buffer* bbuf, uint8_t* arr, size_t len, 
    uint64_t index) {
    for (size_t i = 0; i < len; i++) {
        ako_bytebuf_put_at(bbuf, arr[i], index + i);
    }
}

void ako_bytebuf_put_double(ako_byte_buffer* bbuf, double value) {
    if (bbuf->len + sizeof(double) >= bbuf->size)
        return;
    *(double*)(bbuf->buf + bbuf->len) = value;
    bbuf->len += sizeof(double);
}

inline void ako_bytebuf_put_double_at(ako_byte_buffer* bbuf, double value, uint64_t index) {
    *(double*)(bbuf->buf + index) = value;
}

void ako_bytebuf_put_float(ako_byte_buffer* bbuf, float value) {
    if (bbuf->len + sizeof(float) >= bbuf->size)
        return;
    *(float*)(bbuf->buf + bbuf->len) = value;
    bbuf->len += sizeof(float);
}

inline void ako_bytebuf_put_float_at(ako_byte_buffer* bbuf, float value, uint64_t index) {
    *(float*)(bbuf->buf + index) = value;
}

void ako_bytebuf_put_int(ako_byte_buffer* bbuf, uint32_t value) {
    if (bbuf->len + sizeof(uint32_t) >= bbuf->size)
        return;
    *(uint32_t*)(bbuf->buf + bbuf->len) = value;
    bbuf->len += sizeof(uint32_t);
}

inline void ako_bytebuf_put_int_at(ako_byte_buffer* bbuf, uint32_t value, uint64_t index) {
    *(uint32_t*)(bbuf->buf + index) = value;
}

void ako_bytebuf_put_long(ako_byte_buffer* bbuf, uint64_t value) {
    if (bbuf->len + sizeof(uint64_t) >= bbuf->size)
        return;
    *(uint64_t*)(bbuf->buf + bbuf->len) = value;
    bbuf->len += sizeof(uint64_t);
}

inline void ako_bytebuf_put_long_at(ako_byte_buffer* bbuf, uint64_t value, uint64_t index) {
    *(uint64_t*)(bbuf->buf + index) = value;
}

void ako_bytebuf_put_short(ako_byte_buffer* bbuf, uint16_t value) {
    if (bbuf->len + sizeof(uint16_t) >= bbuf->size)
        return;
    *(uint16_t*)(bbuf->buf + bbuf->len) = value;
    bbuf->len += sizeof(uint16_t);
}

inline void ako_bytebuf_put_short_at(ako_byte_buffer* bbuf, uint16_t value, uint64_t index) {
    *(uint16_t*)(bbuf->buf + index) = value;
}
