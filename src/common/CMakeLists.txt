set(COMMON_SRCS
    spk_bytebuffer.c
    spk_slab.c)

add_library(common_objs STATIC ${COMMON_SRCS})
target_include_directories(common_objs PRIVATE ../include/)
