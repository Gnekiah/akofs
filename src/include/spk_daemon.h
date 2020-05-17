/*
 * Sparkle Daemon Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_DAEMON_H_
#define SPARKLE_SPK_DAEMON_H_

#include <spk_core.h>

extern int spk_daemon_init();

extern int spk_daemon_start();

extern void spk_daemon_exit();

#endif // SPARKLE_SPK_DAEMON_H_