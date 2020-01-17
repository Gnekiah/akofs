/*
 * Sparkle Regional Manager Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: 
 */

#include <spk_core.h>
#include <spk_timer.h>

void cron_timer_demo_fn(void) {
    /* ����һ����ʱ�����demo */
}

void call_back_demo_fn(spk_io_context* context) {
    /* ����һ��������Ϣ�ص������demo */
}

int spk_rms_init(struct spk_rms_ops* rms) {
    int err = 0;
    
    /* ע��һ����ʱ������1���Ӻ�ʼִ�У�ִֻ��һ�� */
    spk_timer_init(cron_timer_demo_fn, 1000);
    /* ע��һ����ʱ������3���Ӻ�ʼִ�У�����ִ����ɺ󣬵ȴ�3�����ٴ�ִ�У��ظ�600�� */
    spk_timer_repeat_init(cron_timer_demo_fn, 3000, 600);

    /* ��ʼ������ڵ��ΨһID */
    rms->rms_id = 0x00112233;
    /* ����RMS�ڵ������ڴ���ping��Ϣ����ں���ָ�� */
    rms->ops.ping_fn = call_back_demo_fn;

    return err;
}

void spk_rms_exit(void) {

}
