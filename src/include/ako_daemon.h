/*
 * AkoFS Daemon Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_SPK_DAEMON_H_
#define AKOFS_SPK_DAEMON_H_

#include <ako_core.h>

extern int ako_daemon_init();

extern int ako_daemon_start();

extern void ako_daemon_exit();

#endif // AKOFS_SPK_DAEMON_H_