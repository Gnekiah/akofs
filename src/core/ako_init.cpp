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
    /* ����һ����ʱ�����demo */
    akolog_error("cron./......");
    ako_timer_init(cron_timer_demo_fn, 1000, handle);
}

void cron_timer_demo2_fn(void* handle) {
    /* ����һ����ʱ�����demo */
    akolog_error("cron./.dfddddddddd.....");
    ako_timer_init(cron_timer_demo_fn, 1000, handle);
}

void call_back_demo_fn(void* handle, uint64_t nread, uint64_t buff_size, char* buff_base) {
    /* ����һ��������Ϣ�ص������demo */
}

int ako_init(struct ako_callback_ops* ops) {
    int err = 0;
    /* �����ڴ���Դ */
    
    /* ע��һ����ʱ������1���Ӻ�ʼִ�У�ִֻ��һ�� */
    ako_timer_init(cron_timer_demo_fn, 1000, nullptr);
    /* ע��һ����ʱ������3���Ӻ�ʼִ�У��ظ�600�� */
    ako_timer_repeat_init(cron_timer_demo2_fn, 10000, 3, nullptr);

    ako_event_callback_init(call_back_demo_fn);
    /* ����RMS�ڵ������ڴ���ping��Ϣ����ں���ָ�� */
    //ops->ping_fn = call_back_demo_fn;

    return err;
}

void ako_exit(void) {
    /* �ͷ��ڴ���Դ */
}
