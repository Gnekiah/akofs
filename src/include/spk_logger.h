/*
 * Header definition for logger interface.
 *
 * logger initialization & release.
 *      extern int spk_logger_init(const char* path);
 *      extern int spk_logger_check_config(const char* path);
 *      extern void spk_logger_free(void);
 * logger interface.
 *      spklog_debug(...);
 *      spklog_info(...);
 *      spklog_notice(...);
 *      spklog_warn(...);
 *      spklog_error(...);
 *      spklog_fatal(...);
 *      spklog_hex_debug(buf, buf_len);
 *      spklog_hex_info(buf, buf_len);
 *      spklog_hex_notice(buf, buf_len);
 *      spklog_hex_warn(buf, buf_len);
 *      spklog_hex_error(buf, buf_len);
 *      spklog_hex_fatal(buf, buf_len);
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_LOG_LOGGER_H_
#define SPARKLE_LOG_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <spk_compat.h>

#include "../log/zlog/zlog.h"

extern zlog_category_t* __spklog_cat_debug__;
extern zlog_category_t* __spklog_cat_info__;
extern zlog_category_t* __spklog_cat_notice__;
extern zlog_category_t* __spklog_cat_warn__;
extern zlog_category_t* __spklog_cat_error__;
extern zlog_category_t* __spklog_cat_fatal__;

extern int spk_logger_init(const char* path);
extern int spk_logger_check_config(const char* path);
extern int spk_logger_reload(const char* path);
extern void spk_logger_free(void);

/*
 * Macro for getting error position inner throwed file
 */
#define LOGGING_POSITION  __FILE__,",",__LINE__,",",__FUNCTION__,": "

#define spklog_debug(...) do { \
    if (likely(__spklog_cat_debug__)) \
        zlog(__spklog_cat_debug__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__); \
} while(0)

#define spklog_info(...) do { \
    if (likely(__spklog_cat_info__)) \
        zlog(__spklog_cat_info__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__); \
} while(0)

#define spklog_notice(...) do { \
    if (likely(__spklog_cat_notice__)) \
        zlog(__spklog_cat_notice__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_NOTICE, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_NOTICE, __VA_ARGS__); \
} while(0)

#define spklog_warn(...) do { \
    if (likely(__spklog_cat_warn__)) \
        zlog(__spklog_cat_warn__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_WARN, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_WARN, __VA_ARGS__); \
} while(0)

#define spklog_error(...) do { \
    if (likely(__spklog_cat_error__)) \
        zlog(__spklog_cat_error__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__); \
} while(0)

#define spklog_fatal(...) do { \
    if (likely(__spklog_cat_fatal__)) \
        zlog(__spklog_cat_fatal__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__); \
    else dzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__); \
} while(0)

#define spklog_hex_debug(buf, buf_len) do { \
    if (likely(__spklog_cat_debug__)) \
        hzlog(__spklog_cat_debug__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_DEBUG, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_DEBUG, buf, buf_len); \
} while(0)

#define spklog_hex_info(buf, buf_len) do { \
    if (likely(__spklog_cat_info__)) \
        hzlog(__spklog_cat_info__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_INFO, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_INFO, buf, buf_len); \
} while(0)

#define spklog_hex_notice(buf, buf_len) do { \
    if (likely(__spklog_cat_notice__)) \
        hzlog(__spklog_cat_notice__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_NOTICE, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_NOTICE, buf, buf_len); \
} while(0)

#define spklog_hex_warn(buf, buf_len) do { \
    if (likely(__spklog_cat_warn__)) \
        hzlog(__spklog_cat_warn__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_WARN, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_WARN, buf, buf_len); \
} while(0)

#define spklog_hex_error(buf, buf_len) do { \
    if (likely(__spklog_cat_error__)) \
        hzlog(__spklog_cat_error__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_ERROR, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_ERROR, buf, buf_len); \
} while(0)

#define spklog_hex_fatal(buf, buf_len) do { \
    if (likely(__spklog_cat_fatal__)) \
        hzlog(__spklog_cat_fatal__, __FILE__, sizeof(__FILE__)-1, __FUNCTION__, \
            sizeof(__FUNCTION__)-1, __LINE__, ZLOG_LEVEL_FATAL, buf, buf_len); \
    else hdzlog(__FILE__, sizeof(__FILE__)-1, __FUNCTION__, sizeof(__FUNCTION__)-1, \
        __LINE__, ZLOG_LEVEL_FATAL, buf, buf_len); \
} while(0)

#ifdef __cplusplus
}
#endif

#endif  // SPARKLE_LOG_LOGGER_H_
