/*
 * AkoFS Regional Manager Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: 
 */

#include <ako_core.h>
#include <ako_eventd.h>
#include <ako_logger.h>


void cron_timer_demo_fn(void* handle) {
    /* 这是一个定时任务的demo */
    akolog_error("cron./......");
    ako_timer_init(cron_timer_demo_fn, 1000, handle);
}

void cron_timer_demo2_fn(void* handle) {
    /* 这是一个定时任务的demo */
    akolog_error("cron./.dfddddddddd.....");
    ako_timer_init(cron_timer_demo_fn, 1000, handle);
}

void call_back_demo_fn(void* handle, uint64_t nread, uint64_t buff_size, char* buff_base) {
    /* 这是一个处理消息回调任务的demo */
}

int ako_init(struct ako_callback_ops* ops) {
    int err = 0;
    /* 申请内存资源 */
    
    /* 注册一个定时任务，在1秒钟后开始执行，只执行一次 */
    ako_timer_init(cron_timer_demo_fn, 1000, nullptr);
    /* 注册一个定时任务，在3秒钟后开始执行，重复600次 */
    ako_timer_repeat_init(cron_timer_demo2_fn, 10000, 3, nullptr);

    ako_event_callback_init(call_back_demo_fn);
    /* 设置RMS节点中用于处理ping消息的入口函数指针 */
    //ops->ping_fn = call_back_demo_fn;

    return err;
}

void ako_exit(void) {
    /* 释放内存资源 */
}
