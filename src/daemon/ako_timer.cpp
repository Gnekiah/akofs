/*
 * timer in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <ako_eventd.h>
#include "ako_eventd_core.h"
#include <vector>

std::vector<uv_timer_t*> timer_vec;

 /*
  * set a timer to perform specified segment
  * fn: task interface, enter for task thread
  * expire_ms: expire time to run the task, in millisecond
  */
int ako_timer_init(ako_crontack_fn fn, uint64_t expire_ms, void* handle) {
    int ret = 0;

    if (handle) {
        ret = uv_timer_start((uv_timer_t*)handle, (uv_timer_cb)fn, expire_ms, 0);
        return ret;
    }

    uv_timer_t* timer_handle = new uv_timer_t;
    
    ret = uv_timer_init(loop, timer_handle);
    ret = uv_timer_start(timer_handle, (uv_timer_cb)fn, expire_ms, 0);

    timer_vec.push_back(timer_handle);
    return ret;
}

/*
 * set a timer to perform specified repeat segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 * repeat: repeat times to run. set `repeat=0` to execute unlimited times
 */
int ako_timer_repeat_init(ako_crontack_fn fn, uint64_t expire_ms, uint64_t repeat, void* handle) {
    int ret = 0;

    if (handle) {
        ret = uv_timer_start((uv_timer_t*)handle, (uv_timer_cb)fn, expire_ms, repeat);
        return ret;
    }

    uv_timer_t* timer_handle = new uv_timer_t;

    ret = uv_timer_init(loop, timer_handle);
    ret = uv_timer_start(timer_handle, (uv_timer_cb)fn, expire_ms, repeat);

    timer_vec.push_back(timer_handle);
    return ret;
}
