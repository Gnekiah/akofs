/*
 * Header definition for logger interface.
 *
 * logger initialization & release.
 *      extern int ako_logger_init(const char* path);
 *      extern int ako_logger_check_config(const char* path);
 *      extern void ako_logger_free(void);
 * logger interface.
 *      akolog_debug(...);
 *      akolog_info(...);
 *      akolog_notice(...);
 *      akolog_warn(...);
 *      akolog_error(...);
 *      akolog_fatal(...);
 *      akolog_hex_debug(buf, buf_len);
 *      akolog_hex_info(buf, buf_len);
 *      akolog_hex_notice(buf, buf_len);
 *      akolog_hex_warn(buf, buf_len);
 *      akolog_hex_error(buf, buf_len);
 *      akolog_hex_fatal(buf, buf_len);
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_LOG_LOGGER_H_
#define AKOFS_LOG_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ako_compat.h>

#include "../common/zlog/zlog.h"

extern zlog_category_t* __akolog_cat_debug__;
extern zlog_category_t* __akolog_cat_info__;
extern zlog_category_t* __akolog_cat_notice__;
extern zlog_category_t* __akolog_cat_warn__;
extern zlog_category_t* __akolog_cat_error__;
extern zlog_category_t* __akolog_cat_fatal__;

extern int ako_logger_init(const char* path);
extern int ako_logger_check_config(const char* path);
extern int ako_logger_reload(const char* path);
extern void ako_logger_free(void);

/*
 * Macro for getting error position inner throwed file
 */
#define LOGGING_POSITION  __FILE__,",",__LINE__,",",__FUNCTION__,": "

#define akolog_debug(...) do { \
    if (likely(__akolog_cat_debug__)) \
        zlog(__akolog_cat_debug__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__); \
} while(0)

#define akolog_info(...) do { \
    if (likely(__akolog_cat_info__)) \
        zlog(__akolog_cat_info__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__); \
} while(0)

#define akolog_notice(...) do { \
    if (likely(__akolog_cat_notice__)) \
        zlog(__akolog_cat_notice__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_NOTICE, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_NOTICE, __VA_ARGS__); \
} while(0)

#define akolog_warn(...) do { \
    if (likely(__akolog_cat_warn__)) \
        zlog(__akolog_cat_warn__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_WARN, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_WARN, __VA_ARGS__); \
} while(0)

#define akolog_error(...) do { \
    if (likely(__akolog_cat_error__)) \
        zlog(__akolog_cat_error__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__); \
} while(0)

#define akolog_fatal(...) do { \
    if (likely(__akolog_cat_fatal__)) \
        zlog(__akolog_cat_fatal__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__); \
} while(0)

#define akolog_hex_debug(buf, buf_len) do { \
    if (likely(__akolog_cat_debug__)) \
        hzlog(__akolog_cat_debug__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_DEBUG, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_DEBUG, buf, buf_len); \
} while(0)

#define akolog_hex_info(buf, buf_len) do { \
    if (likely(__akolog_cat_info__)) \
        hzlog(__akolog_cat_info__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_INFO, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_INFO, buf, buf_len); \
} while(0)

#define akolog_hex_notice(buf, buf_len) do { \
    if (likely(__akolog_cat_notice__)) \
        hzlog(__akolog_cat_notice__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_NOTICE, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_NOTICE, buf, buf_len); \
} while(0)

#define akolog_hex_warn(buf, buf_len) do { \
    if (likely(__akolog_cat_warn__)) \
        hzlog(__akolog_cat_warn__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_WARN, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_WARN, buf, buf_len); \
} while(0)

#define akolog_hex_error(buf, buf_len) do { \
    if (likely(__akolog_cat_error__)) \
        hzlog(__akolog_cat_error__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_ERROR, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_ERROR, buf, buf_len); \
} while(0)

#define akolog_hex_fatal(buf, buf_len) do { \
    if (likely(__akolog_cat_fatal__)) \
        hzlog(__akolog_cat_fatal__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_FATAL, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_FATAL, buf, buf_len); \
} while(0)

#ifdef __cplusplus
}
#endif

#endif  // AKOFS_LOG_LOGGER_H_
