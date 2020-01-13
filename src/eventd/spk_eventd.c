/*
 * Sparkle Event Drive Module
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_eventd.h>
#include <assert.h>
#include <spk_logger.h>
#include "eventd_core.h"

static void __spk_libuv_close_walk_cb(uv_handle_t* handle, void* arg) {
    if (!uv_is_closing(handle))
        uv_close(handle, NULL);
}

static void __spk_libuv_close_loop(uv_loop_t* loop) {
    spklog_debug(LOGGING_POSITION, "uv_loop closed");
    uv_walk(loop, __spk_libuv_close_walk_cb, NULL);
    uv_run(loop, UV_RUN_DEFAULT);
}

int spk_eventd_init(const struct eventd_config_t* config) {
    int err = 0;

    if (loop == NULL) {
        spklog_debug(LOGGING_POSITION, "uv_loop init from uv_default");
        loop = uv_default_loop();
    }

    err = spk_server_css_init(config);
    if (err) {
        goto err_css;
    }
    err = spk_server_das_init(config);
    if (err) {
        goto err_das;
    }



    goto out;

err_das:
    spk_server_css_exit();

err_css:
    __spk_libuv_close_loop(loop);
    uv_is_closing((uv_handle_t*)loop);
    uv_loop_close(loop);
    loop = NULL;
out:
    return err;
}

void spk_eventd_exit() {
    spk_server_das_exit();
    spk_server_css_exit();

    __spk_libuv_close_loop(loop);
    uv_is_closing((uv_handle_t*)loop);
    uv_loop_close(loop);
    loop = NULL;
}

void spk_eventd_loop() {
    assert(loop);
    uv_run(loop, UV_RUN_DEFAULT);
}