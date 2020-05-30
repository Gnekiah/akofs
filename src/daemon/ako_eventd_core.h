/*
 * event drives, private segment
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_EVENTD_CORE_H_
#define AKOFS_EVENTD_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <uv.h>
#include <ako_eventd.h>

extern uv_loop_t* loop;

typedef void (eventd_data_handle_fn)(struct eventd_io_context_t*, void* data, uint64_t size);

extern int ako_server_init(const struct eventd_config_t*);
extern void ako_server_exit();

#ifdef __cplusplus
}
#endif

#endif // AKOFS_EVENTD_CORE_H_