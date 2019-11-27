/*
 * spk_hash.c -- md4/md5/sha1/sha224/sha256/sha386/sha512
 * based on libcrypto
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_compto.h>

#include <openssl/md4.h>
#include <openssl/md5.h>
#ifdef HAVE_OPENSSL_MDC2_H
#include <openssl/mdc2.h>
#endif
#include <openssl/sha.h>

inline uint8_t* spk_md4(const uint8_t* src, uint64_t src_size, uint8_t* dst) {
    return MD4(src, src_size, dst);
}

inline uint8_t* spk_md5(const uint8_t* src, uint64_t src_size, uint8_t* dst) {
    return MD5(src, src_size, dst);
}

inline uint8_t* spk_mdc2(const uint8_t* src, uint64_t src_size, uint8_t* dst) {
#ifdef HAVE_OPENSSL_MDC2_H
    return MDC2(src, src_size, dst);
#else
    return MD5(src, src_size, dst);
#endif
}

inline uint8_t* spk_sha1(const uint8_t* src, uint64_t src_size, uint8_t* dst) {
    return SHA1(src, src_size, dst);
}

inline uint8_t* spk_sha224(const uint8_t* src, uint64_t src_size, uint8_t* dst) {
    return SHA224(src, src_size, dst);
}

inline uint8_t* spk_sha256(const uint8_t* src, uint64_t src_size, uint8_t* dst) {
    return SHA256(src, src_size, dst);
}

inline uint8_t* spk_sha384(const uint8_t* src, uint64_t src_size, uint8_t* dst) {
    return SHA384(src, src_size, dst);
}

inline uint8_t* spk_sha512(const uint8_t* src, uint64_t src_size, uint8_t* dst) {
    return SHA512(src, src_size, dst);
}
