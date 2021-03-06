/*
 * AkoFS Daemon Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <ako_daemon.h>
#include <ako_eventd.h>
#include "ako_eventd_core.h"

static void __close_walk_cb(uv_handle_t* handle, void* arg) {
    if (!uv_is_closing(handle))
        uv_close(handle, NULL);

}
static void __close_loop(uv_loop_t* loop) {
    uv_walk(loop, __close_walk_cb, NULL);
    uv_run(loop, UV_RUN_DEFAULT);
}

static int el_psy_congroo() {
    int ret = 0;

    ret = uv_run(loop, UV_RUN_DEFAULT);

    /* close loop */
    __close_loop(loop);
    ret = uv_is_closing((uv_handle_t*)loop);
    ret = uv_loop_close(loop);

    return ret;
}

int ako_daemon_init() {
    int err = 0;

/* init rms/das/css in main.c */
/*
    struct ako_rms_ops *rms_ops = nullptr;
    struct ako_das_ops *das_ops = nullptr;
    struct ako_css_ops *css_ops = nullptr;

    rms_ops = new struct ako_rms_ops;
    das_ops = new struct ako_das_ops;
    css_ops = new struct ako_css_ops;

    if (!(rms_ops && das_ops && css_ops)) {
        akolog_fatal(LOGGING_POSITION, "failed to alloc `ops struct`");
        err = SPKE_NOMEM;
        goto out;
    }

    err = ako_das_init(das_ops);
    if (err) {
        akolog_fatal(LOGGING_POSITION, "failed to init das");
        goto err_das_init;
    }

    err = ako_rms_init(rms_ops);
    if (err) {
        akolog_fatal(LOGGING_POSITION, "failed to init rms");
        goto err_rms_init;
    }

    err = ako_css_init(css_ops);
    if (err) {
        akolog_fatal(LOGGING_POSITION, "failed to init css");
        goto err_css_init;
    }

    /// TODO: set ops to msgon
    goto out;

err_css_init:
    ako_css_exit();
err_rms_init:
    ako_rms_exit();
err_das_init:
    ako_das_exit();
out:
    delete rms_ops;
    delete das_ops;
    delete css_ops;
*/
    return err;
}

int ako_daemon_start() {
    return el_psy_congroo();
}

void ako_daemon_exit() {

}