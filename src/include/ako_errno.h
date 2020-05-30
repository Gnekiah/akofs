/*
 * AkoFS Return Error Defination
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_AKO_ERRNO_H_
#define AKOFS_AKO_ERRNO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define AKOE_NOMEM                              101

#define AKOE_EVENTD_LOOP_NULL                   201
#define AKOE_EVENTD_TCP_INIT_ERROR              202
#define AKOE_EVENTD_ADDR_ERROR                  203
#define AKOE_EVENTD_TCP_BIND_ERROR              204
#define AKOE_EVENTD_TCP_LISTEN_ERROR            205

#define AKOE_MESSAGE_DUMP_DATA_INSUFFICIENT     301
#define AKOE_MESSAGE_DUMP_PACKAGE_ERROR         302

#ifdef __cplusplus
}
#endif

#endif // AKOFS_AKO_ERRNO_H_