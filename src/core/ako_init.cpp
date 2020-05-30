/*
 * Sparkle Regional Manager Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: 
 */

#include <ako_core.h>
#include <ako_eventd.h>

void cron_timer_demo_fn(void) {
    /* ����һ����ʱ�����demo */
}

void call_back_demo_fn(ako_io_context* context) {
    /* ����һ��������Ϣ�ص������demo */
}

int ako_init(struct ako_callback_ops* ops) {
    int err = 0;
    /* �����ڴ���Դ */
    
    /* ע��һ����ʱ������1���Ӻ�ʼִ�У�ִֻ��һ�� */
    ako_timer_init(cron_timer_demo_fn, 1000);
    /* ע��һ����ʱ������3���Ӻ�ʼִ�У�����ִ����ɺ󣬵ȴ�3�����ٴ�ִ�У��ظ�600�� */
    ako_timer_repeat_init(cron_timer_demo_fn, 3000, 600);

    /* ����RMS�ڵ������ڴ���ping��Ϣ����ں���ָ�� */
    ops->ping_fn = call_back_demo_fn;

    return err;
}

void ako_exit(void) {
    /* �ͷ��ڴ���Դ */
}
