/*
 * spk_aes.c
 * based on libcrypto
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_compto.h>
#include <spk_compat.h>
#include <string.h>
#include <openssl/aes.h>

inline uint64_t spk_aes_bound(uint64_t size) {
    return ((size >> 4) << 4) + (size & 0x0f ? 0x10 : 0);
}

uint64_t spk_aes_cbc256_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    uint64_t dst_size, const char* key) {
    const uint8_t* src_end = src + src_size;
    uint8_t* dst_end = dst;
    uint8_t in[AES_BLOCK_SIZE];
    
    if (unlikely(!dst || !key || !src || !src_size)) {
        return -1;
    }

    AES_KEY aes;
    if (AES_set_encrypt_key((uint8_t*)key, 256, &aes) < 0){
        return -2;
    }

    if (dst_size < spk_aes_bound(src_size)) {
        return -3;
    }

    while (src_end - src >= AES_BLOCK_SIZE) {
        memcpy(in, src, AES_BLOCK_SIZE);
        AES_encrypt(in, dst_end, &aes);
        src += AES_BLOCK_SIZE;
        dst_end += AES_BLOCK_SIZE;
    }

    if (src_end != src) {
        memset(in, 0, AES_BLOCK_SIZE);
        memcpy(in, src, MIN(AES_BLOCK_SIZE, src_end - src));
        AES_encrypt(in, dst_end, &aes);
        src = src_end;
        dst_end += AES_BLOCK_SIZE;
    }

    return dst_end - dst;
}

uint64_t spk_aes_cbc256_decrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    uint64_t dst_size, const char* key) {
    const uint8_t* src_end = src + src_size;
    uint8_t* dst_end = dst;
    uint8_t in[AES_BLOCK_SIZE];
    
    if (unlikely(!dst || !key || !src || !src_size || dst_size < src_size)) {
        return -1;
    }

    AES_KEY aes;
    if (AES_set_decrypt_key((uint8_t*)key, 256, &aes) < 0) {
        return -2;
    }

    while (src_end - src >= AES_BLOCK_SIZE) {
        memcpy(in, src, AES_BLOCK_SIZE);
        AES_decrypt(in, dst_end, &aes);
        src += AES_BLOCK_SIZE;
        dst_end += AES_BLOCK_SIZE;
    }

    if (src_end != src) {
        memset(in, 0, AES_BLOCK_SIZE);
        memcpy(in, src, MIN(AES_BLOCK_SIZE, src_end - src));
        AES_decrypt(in, dst_end, &aes);
        src = src_end;
        dst_end += AES_BLOCK_SIZE;
    }

    return dst_end - dst;
}

