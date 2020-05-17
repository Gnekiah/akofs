/*
 * Sparkle Daemon Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_daemon.h>

static int el_psy_congroo() {
    int ret = 0;

    return ret;
}

int spk_daemon_init() {
    int err = 0;

/* init rms/das/css in main.c */
/*
    struct spk_rms_ops *rms_ops = nullptr;
    struct spk_das_ops *das_ops = nullptr;
    struct spk_css_ops *css_ops = nullptr;

    rms_ops = new struct spk_rms_ops;
    das_ops = new struct spk_das_ops;
    css_ops = new struct spk_css_ops;

    if (!(rms_ops && das_ops && css_ops)) {
        spklog_fatal(LOGGING_POSITION, "failed to alloc `ops struct`");
        err = SPKE_NOMEM;
        goto out;
    }

    err = spk_das_init(das_ops);
    if (err) {
        spklog_fatal(LOGGING_POSITION, "failed to init das");
        goto err_das_init;
    }

    err = spk_rms_init(rms_ops);
    if (err) {
        spklog_fatal(LOGGING_POSITION, "failed to init rms");
        goto err_rms_init;
    }

    err = spk_css_init(css_ops);
    if (err) {
        spklog_fatal(LOGGING_POSITION, "failed to init css");
        goto err_css_init;
    }

    /// TODO: set ops to msgon
    goto out;

err_css_init:
    spk_css_exit();
err_rms_init:
    spk_rms_exit();
err_das_init:
    spk_das_exit();
out:
    delete rms_ops;
    delete das_ops;
    delete css_ops;
*/
    return err;
}

int spk_daemon_start() {
    return el_psy_congroo();
}

void spk_daemon_exit() {
    spk_css_exit();
    spk_rms_exit();
    spk_das_exit();
}