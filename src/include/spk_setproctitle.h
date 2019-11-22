/*
 * Header definition for renaming process title.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_SET_PROCTITLE_H_
#define SPARKLE_SPK_SET_PROCTITLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(HAVE_SETPROCTITLE)
#define spk_setproctitle(argv, title) setproctitle("%s", title)

#else
#if !defined(SETPROCTITLE_USES_ENV)
#if defined(__linux__)
#define SETPROCTITLE_USES_ENV  1
int spk_setproctitle(char** argv, const char* title);

#else
#define spk_setproctitle(argv, title) 0

#endif
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_SET_PROCTITLE_H_
