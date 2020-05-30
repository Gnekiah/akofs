/*
 * AkoFS compress and crypto module.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_SPK_COMPTO_H_
#define AKOFS_SPK_COMPTO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * Compresses the source buffer into the destination buffer. src_size is the byte 
 * length of the source buffer. Upon entry, dst_size is the total size of the 
 * destination buffer, which must be at least the value returned by 
 * ako_compress_bound(src_size). Upon exit, dst_size is the actual size of the 
 * compressed data. 
 * ako_compress returns Z_OK if success, Z_MEM_ERROR if there was not enough memory, 
 * Z_BUF_ERROR if there was not enough room in the output buffer.
 */
extern int ako_compress(uint8_t* dst, uint64_t* dst_size, uint8_t* src, uint64_t src_size);

/*
 * Decompresses the source buffer into the destination buffer. src_size is the byte 
 * length of the source buffer. Upon entry, dst_size is the total size of the 
 * destination buffer, which must be large enough to hold the entire uncompressed 
 * data. (The size of the uncompressed data must have been saved previously by the 
 * compressor and transmitted to the decompressor by some mechanism outside the 
 * scope of this compression library.) Upon exit, destLen is the actual size of the 
 * uncompressed data.
 * ako_uncompress returns Z_OK if success, Z_MEM_ERROR if there was not enough 
 * memory, Z_BUF_ERROR if there was not enough room in the output buffer, or 
 * Z_DATA_ERROR if the input data was corrupted or incomplete. In the case where 
 * there is not enough room, ako_uncompress() will fill the output buffer with 
 * the uncompressed data up to that point.
 */
extern int ako_uncompress(uint8_t* dst, uint64_t* dst_size, uint8_t* src, uint64_t src_size);

/*
 * ako_compress_bound() returns an upper bound on the compressed size after ako_compress()
 * on size bytes. It would be used before a ako_compress() call to allocate the 
 * destination buffer.
 */
extern uint64_t ako_compress_bound(uint64_t size);

/*
 * Update a running CRC-32 with the bytes buf[0..len-1] and return the updated CRC-32. 
 * If buf is NULL, this function returns the required initial value for the crc. 
 * Pre- and post-conditioning (one's complement) is performed within this function so 
 * it shouldn't be done by the application.
 * Usage example:
 *     uLong crc = crc32(0L, Z_NULL, 0);
 *     while (read_buffer(buffer, length) != EOF) {
 *       crc = crc32(crc, buffer, length);
 *     }
 */
extern uint32_t ako_crc32(uint32_t crc, const uint8_t* buf, uint32_t len);

/*
 * Combine two CRC-32 check values into one. For two sequences of bytes, seq1 and seq2
 * with lengths len1 and len2, CRC-32 check values were calculated for each, crc1 and 
 * crc2. ako_crc32_combine() returns the CRC-32 check value of seq1 and seq2 concatenated, 
 * requiring only crc1, crc2, and len2.
 */
extern uint32_t ako_crc32_combine(uint32_t crc1, uint32_t crc2, uint32_t len2);

// 128 bit
extern uint8_t* ako_md4(const uint8_t* src, uint64_t src_size, uint8_t* dst);
// 128 bit
extern uint8_t* ako_md5(const uint8_t* src, uint64_t src_size, uint8_t* dst);
// 128 bit
extern uint8_t* ako_mdc2(const uint8_t* src, uint64_t src_size, uint8_t* dst);
// 160 bit
extern uint8_t* ako_sha1(const uint8_t* src, uint64_t src_size, uint8_t* dst);
// 224 bit
extern uint8_t* ako_sha224(const uint8_t* src, uint64_t src_size, uint8_t* dst);
// 256 bit
extern uint8_t* ako_sha256(const uint8_t* src, uint64_t src_size, uint8_t* dst);
// 384 bit
extern uint8_t* ako_sha384(const uint8_t* src, uint64_t src_size, uint8_t* dst);
// 512 bit
extern uint8_t* ako_sha512(const uint8_t* src, uint64_t src_size, uint8_t* dst);

/*
 * out is null-terminated encode string.
 * return values is out length, exclusive terminating `\0'
 */
extern uint64_t ako_base64_encode(const uint8_t* src, uint64_t src_size, int8_t* dst);
extern uint64_t ako_base64_enbound(uint64_t size);
/*
 * return values is out length
 */
extern uint64_t ako_base64_decode(const int8_t* src, uint64_t src_size, uint8_t* dst);
extern uint64_t ako_base64_debound(uint64_t size);


extern uint64_t ako_des_bound(uint64_t size);
extern uint64_t ako_des_ecb_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key);
extern uint64_t ako_des_ecb_decrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key);
extern uint64_t ako_des_ncbc_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key);
extern uint64_t ako_des_ncbc_decrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    const char* key);


extern int ako_rsa_generate_keypair_2048(int8_t* pri_key, uint64_t* pri_key_size,
    int8_t* pub_key, uint64_t* pub_key_size);
extern uint64_t ako_rsa_pub_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    uint64_t dst_size, const int8_t* pub_key);
extern uint64_t ako_rsa_pri_decrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    uint64_t dst_size, const int8_t* pri_key);
extern uint64_t ako_rsa_pri_sign(const uint8_t* src, const uint64_t src_size, int8_t* sign,
    uint64_t sign_alloc_size, const int8_t* pri_key);
extern int ako_rsa_pub_verify(const uint8_t* src, const uint64_t src_size, const int8_t* sign,
    const uint64_t sign_size, const int8_t* pub_key);


extern uint64_t ako_aes_bound(uint64_t size);
extern uint64_t ako_aes_cbc256_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    uint64_t dst_size, const char* key);
extern uint64_t ako_aes_cbc256_decrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    uint64_t dst_size, const char* key);

#ifdef __cplusplus
}
#endif

#endif // AKOFS_SPK_COMPTO_H_