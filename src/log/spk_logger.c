/*
 * Source for logger module.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_logger.h>
#include <stdlib.h>

zlog_category_t* __spklog_cat_debug__ = NULL;
zlog_category_t* __spklog_cat_info__ = NULL;
zlog_category_t* __spklog_cat_notice__ = NULL;
zlog_category_t* __spklog_cat_warn__ = NULL;
zlog_category_t* __spklog_cat_error__ = NULL;
zlog_category_t* __spklog_cat_fatal__ = NULL;

int spk_logger_init(const char* path) {
    int err = zlog_init(path);
    if (err) {
        return -1;
    }
    __spklog_cat_debug__ = zlog_get_category("cat_debug");
    __spklog_cat_info__ = zlog_get_category("cat_info");
    __spklog_cat_notice__ = zlog_get_category("cat_notice");
    __spklog_cat_warn__ = zlog_get_category("cat_warn");
    __spklog_cat_error__ = zlog_get_category("cat_error");
    __spklog_cat_fatal__ = zlog_get_category("cat_fatal");

    if (__spklog_cat_debug__ && __spklog_cat_info__ && 
        __spklog_cat_notice__ && __spklog_cat_warn__ && 
        __spklog_cat_error__ && __spklog_cat_fatal__) {
        return 0;
    }
    zlog_fini();
    return -2;
}

int spk_logger_check_config(const char* path) {

#ifdef _MSC_VER
    _putenv_s("ZLOG_PROFILE_ERROR", "d:\\stderr");
    _putenv_s("ZLOG_CHECK_FORMAT_RULE", "1");
#elif __linux__
    setenv("ZLOG_PROFILE_ERROR", "/dev/stderr", 1);
    setenv("ZLOG_CHECK_FORMAT_RULE", "1", 1);
#endif

    int err = zlog_init(path);
    if (err) {
        fprintf(stderr, "\n---[%s] syntax error, see error message above\n", path);
        return -1;
    }
    zlog_fini();
    fprintf(stderr, "--[%s] syntax right\n", path);
    return 0;
}

int spk_logger_reload(const char* path) {
    return zlog_reload(path);
}

void spk_logger_free(void) {
    zlog_fini();
}