/*
 * spk_des.c -- cbc/ecb/ede/ofb
 * based on libcrypto
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_compto.h>
#include <spk_compat.h>
#include <string.h>
#include <openssl/des.h>

inline uint64_t spk_des_bound(uint64_t size) {
    return ((size >> 3) << 3) + (size & 0x07 ? 0x08 : 0);
    //return size / 8 * 8 + (size % 8 ? 8 : 0);
}

static uint64_t __spk_des_ecb_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key, int flag) {
    DES_cblock key_blk;
    DES_key_schedule key_sched;
    const_DES_cblock in;
    uint8_t* dst_end = dst;
    uint8_t* src_end = src + src_size;

    if (unlikely(!dst || !key || !src || !src_size))
        return 0;

    memset(key_blk, 0, 8);
    memcpy(key_blk, key, MIN(8, strlen(key)));

    DES_set_key_unchecked(&key_blk, &key_sched);
    
    while (src_end - src >= 8) {
        memcpy(in, src, 8);
        DES_ecb_encrypt(&in, dst_end, &key_sched, flag);
        src += 8;
        dst_end += 8;
    }
    if (src_end != src) {
        memset(in, 0, 8);
        memcpy(in, src, MIN(8, src_end - src));
        DES_ecb_encrypt(&in, dst_end, &key_sched, flag);
        src = src_end;
        dst_end += 8;
    }
    
    return dst_end - dst;
}

inline uint64_t spk_des_ecb_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key) {
    return __spk_des_ecb_encrypt(src, src_size, dst, key, DES_ENCRYPT);
}

inline uint64_t spk_des_ecb_decrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key) {
    return __spk_des_ecb_encrypt(src, src_size, dst, key, DES_DECRYPT);
}

static uint64_t __spk_des_ncbc_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key, int flag) {
    uint8_t* dst_end = dst;
    uint8_t* src_end = src + src_size;
    DES_cblock key_blk, ivec;
    DES_key_schedule key_sched;

    if (unlikely(!dst || !key || !src || !src_size))
        return 0;

    memset(key_blk, 0, 8);
    memcpy(key_blk, key, MIN(8, strlen(key)));
    DES_set_key_unchecked(&key_blk, &key_sched);
    memset(ivec, 0, sizeof(ivec));

    DES_ncbc_encrypt(src, dst, src_size, &key_sched, &ivec, flag);
    return spk_des_bound(src_size);
}

inline uint64_t spk_des_ncbc_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key) {
    return __spk_des_ncbc_encrypt(src, src_size, dst, key, DES_ENCRYPT);
}

inline uint64_t spk_des_ncbc_decrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key) {
    return __spk_des_ncbc_encrypt(src, src_size, dst, key, DES_DECRYPT);
}
