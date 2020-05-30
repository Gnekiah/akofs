set(COMMON_SRCS
    ako_bytebuffer.c
    ako_slab.c
    ako_getopt.c

    inih/ini.c
    inih/INIReader.cpp
    Config.cpp
    ako_config.cpp

    ako_aes.c
    ako_base64.c
    ako_crc32.c
    ako_compress.c
    ako_des.c
    ako_hash.c
    ako_rsa.c

    zlog/buf.c
    zlog/category.c
    zlog/category_table.c
    zlog/conf.c
    zlog/event.c
    zlog/format.c
    zlog/level.c
    zlog/level_list.c
    zlog/mdc.c
    zlog/record.c
    zlog/record_table.c
    zlog/rotater.c
    zlog/rule.c
    zlog/spec.c
    zlog/thread.c
    zlog/zc_arraylist.c
    zlog/zc_hashtable.c
    zlog/zc_profile.c
    zlog/zc_util.c
    zlog/zlog.c
    ako_logger.c)

add_library(common_objs STATIC ${COMMON_SRCS})
target_include_directories(common_objs PRIVATE ../include/)
