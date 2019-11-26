/*
 * Sparkle compress and crypto module.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_COMPTO_H_
#define SPARKLE_SPK_COMPTO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * Compresses the source buffer into the destination buffer. src_size is the byte 
 * length of the source buffer. Upon entry, dst_size is the total size of the 
 * destination buffer, which must be at least the value returned by 
 * spk_compress_bound(src_size). Upon exit, dst_size is the actual size of the 
 * compressed data. 
 * spk_compress returns Z_OK if success, Z_MEM_ERROR if there was not enough memory, 
 * Z_BUF_ERROR if there was not enough room in the output buffer.
 */
extern int spk_compress(uint8_t* dst, uint64_t* dst_size, uint8_t* src, uint64_t src_size);

/*
 * Decompresses the source buffer into the destination buffer. src_size is the byte 
 * length of the source buffer. Upon entry, dst_size is the total size of the 
 * destination buffer, which must be large enough to hold the entire uncompressed 
 * data. (The size of the uncompressed data must have been saved previously by the 
 * compressor and transmitted to the decompressor by some mechanism outside the 
 * scope of this compression library.) Upon exit, destLen is the actual size of the 
 * uncompressed data.
 * spk_uncompress returns Z_OK if success, Z_MEM_ERROR if there was not enough 
 * memory, Z_BUF_ERROR if there was not enough room in the output buffer, or 
 * Z_DATA_ERROR if the input data was corrupted or incomplete. In the case where 
 * there is not enough room, spk_uncompress() will fill the output buffer with 
 * the uncompressed data up to that point.
 */
extern int spk_uncompress(uint8_t* dst, uint64_t* dst_size, uint8_t* src, uint64_t src_size);

/*
 * spk_compress_bound() returns an upper bound on the compressed size after spk_compress()
 * on size bytes. It would be used before a spk_compress() call to allocate the 
 * destination buffer.
 */
extern uint64_t spk_compress_bound(uint64_t size);

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
extern uint32_t spk_crc32(uint32_t crc, const uint8_t* buf, uint32_t len);

/*
 * Combine two CRC-32 check values into one. For two sequences of bytes, seq1 and seq2
 * with lengths len1 and len2, CRC-32 check values were calculated for each, crc1 and 
 * crc2. spk_crc32_combine() returns the CRC-32 check value of seq1 and seq2 concatenated, 
 * requiring only crc1, crc2, and len2.
 */
extern uint32_t spk_crc32_combine(uint32_t crc1, uint32_t crc2, uint32_t len2);

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_COMPTO_H_