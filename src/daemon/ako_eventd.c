/*
 * AkoFS Event Drive Module
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <ako_eventd.h>
#include <assert.h>
#include <ako_logger.h>
#include "ako_eventd_core.h"

uv_loop_t* loop = NULL;

static void __ako_libuv_close_walk_cb(uv_handle_t* handle, void* arg) {
    if (!uv_is_closing(handle))
        uv_close(handle, NULL);
}

static void __ako_libuv_close_loop(uv_loop_t* loop) {
    akolog_debug(LOGGING_POSITION, "uv_loop closed");
    uv_walk(loop, __ako_libuv_close_walk_cb, NULL);
    uv_run(loop, UV_RUN_DEFAULT);
}

int ako_eventd_init(const struct eventd_config_t* config) {
    int err = 0;

    if (loop == NULL) {
        akolog_debug(LOGGING_POSITION, "uv_loop init from uv_default");
        loop = uv_default_loop();
    }

    err = ako_server_init(config);
    if (err) {
        goto err_css;
    }

    goto out;

err_css:
    ako_server_exit();
    __ako_libuv_close_loop(loop);
    uv_is_closing((uv_handle_t*)loop);
    uv_loop_close(loop);
    loop = NULL;
out:
    return err;
}

void ako_eventd_exit() {
    ako_server_exit();

    __ako_libuv_close_loop(loop);
    uv_is_closing((uv_handle_t*)loop);
    uv_loop_close(loop);
    loop = NULL;
}

void ako_eventd_loop() {
    assert(loop);
    uv_run(loop, UV_RUN_DEFAULT);
}