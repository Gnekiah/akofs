/*
 * Sparkle Return Error Defination
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_ERRNO_H_
#define SPARKLE_SPK_ERRNO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SPKE_NOMEM                              101

#define SPKE_EVENTD_LOOP_NULL                   201
#define SPKE_EVENTD_TCP_INIT_ERROR              202
#define SPKE_EVENTD_ADDR_ERROR                  203
#define SPKE_EVENTD_TCP_BIND_ERROR              204
#define SPKE_EVENTD_TCP_LISTEN_ERROR            205

#define SPKE_MESSAGE_DUMP_DATA_INSUFFICIENT     301
#define SPKE_MESSAGE_DUMP_PACKAGE_ERROR         302

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_ERRNO_H_