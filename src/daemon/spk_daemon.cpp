/*
 * Sparkle Daemon Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_daemon.h>
#include <spk_rms.h>
#include <spk_css.h>
#include <spk_das.h> 

static int el_psy_congroo() {
    int ret = 0;

    return ret;
}

int spk_daemon_init() {
    spk_rms_init();
}

int spk_daemon_start() {
    return el_psy_congroo();
}

void spk_daemon_exit() {

}