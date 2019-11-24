set(LOG_SRCS
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
    spk_logger.c)

add_library(log_objs STATIC ${LOG_SRCS})
target_include_directories(log_objs PRIVATE ../include/)
