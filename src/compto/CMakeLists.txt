set(COMPTO_SRCS
    spk_crc32.c
    spk_compress.c)

add_library(compto_objs STATIC ${COMPTO_SRCS})
target_include_directories(compto_objs PRIVATE ../include/)
