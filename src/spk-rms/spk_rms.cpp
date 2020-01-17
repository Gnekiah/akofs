/*
 * Sparkle Regional Manager Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: 
 */

#include <spk_core.h>
#include <spk_timer.h>

void cron_timer_demo_fn(void) {
    /* 这是一个定时任务的demo */
}

void call_back_demo_fn(spk_io_context* context) {
    /* 这是一个处理消息回调任务的demo */
}

int spk_rms_init(struct spk_rms_ops* rms) {
    int err = 0;
    
    /* 注册一个定时任务，在1秒钟后开始执行，只执行一次 */
    spk_timer_init(cron_timer_demo_fn, 1000);
    /* 注册一个定时任务，在3秒钟后开始执行，任务执行完成后，等待3秒钟再次执行，重复600次 */
    spk_timer_repeat_init(cron_timer_demo_fn, 3000, 600);

    /* 初始化虚拟节点的唯一ID */
    rms->rms_id = 0x00112233;
    /* 设置RMS节点中用于处理ping消息的入口函数指针 */
    rms->ops.ping_fn = call_back_demo_fn;

    return err;
}

void spk_rms_exit(void) {

}
