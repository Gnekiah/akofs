/*
 * event drives, private segment
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_EVENTD_CORE_H_
#define SPARKLE_EVENTD_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <uv.h>
#include <spk_eventd.h>

uv_loop_t* loop = NULL;

typedef void (eventd_data_handle_fn)(struct eventd_io_context_t*, void* data, uint64_t size);


struct spk_rms_ops {
    uint64_t rms_id;
};

extern int spk_server_css_init(const struct eventd_config_t*);
extern void spk_server_css_exit();
extern int spk_server_das_init(const struct eventd_config_t*);
extern void spk_server_das_exit();

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_EVENTD_CORE_H_