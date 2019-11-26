#include <spk_compto.h>

#include <zlib.h>

int spk_compress(uint8_t* dst, uint64_t* dst_size, uint8_t* src, uint64_t src_size) {
    int ret;
    ret = compress(dst, (uLongf*)dst_size, src, (uLongf)src_size);
    if (ret != Z_OK) {
        
    }
    return ret;
}

int spk_uncompress(uint8_t* dst, uint64_t* dst_size, uint8_t* src, uint64_t src_size) {
    int ret;
    ret = uncompress(dst, (uLongf*)dst_size, src, (uLongf)src_size);
    if (ret != Z_OK) {

    }
    return ret;
}

uint64_t spk_compress_bound(uint64_t size) {
    return size + (size >> 12) + (size >> 14) + (size >> 25) + 13;
}