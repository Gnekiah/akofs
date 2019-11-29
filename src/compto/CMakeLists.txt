set(COMPTO_SRCS
    spk_aes.c
    spk_base64.c
    spk_crc32.c
    spk_compress.c
    spk_des.c
    spk_hash.c
    spk_rsa.c)

add_library(compto_objs STATIC ${COMPTO_SRCS})
target_include_directories(compto_objs PRIVATE ../include/)
