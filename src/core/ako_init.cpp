/*
 * AkoFS Regional Manager Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: 
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
 /* for fix `error C2375`: redefinition errors in WinSock2.h */
#define WIN32_LEAN_AND_MEAN
#endif

#include <ako_core.h>
#include <ako_eventd.h>
#include <ako_logger.h>

void cron_timer_demo_fn(void* handle) {
    /* ����һ����ʱ�����demo */
    akolog_info("timer runed");

    struct eventd_io_context *ioc = new struct eventd_io_context;
    
    strcpy(ioc->addr, "127.0.0.1");
    ioc->port = 20530;
    
    ioc->buff_base = new char[1024];
    ioc->handle = nullptr;
    ioc->buff_size = 0;

    strcpy(ioc->buff_base, "wwwwwwwwwwwwwwwwwww");
    ioc->buff_size = sizeof("wwwwwwwwwwwwwwwwwww") + 1;

    ako_socket_send(ioc);

    //ako_timer_init(cron_timer_demo_fn, 1000, handle);
}

//void cron_timer_demo2_fn(void* handle) {
//    /* ����һ����ʱ�����demo */
//    akolog_error("cron./.dfddddddddd.....");
//    ako_timer_init(cron_timer_demo_fn, 1000, handle);
//}

void call_back_demo_fn(void* handle, uint64_t nread, uint64_t buff_size, char* buff_base) {
    akolog_info("callback runed");

    char buff[1024];
    memcpy(buff, buff_base, nread);
    buff[nread] = '\0';
    akolog_info(buff);

    struct eventd_io_context* ioc = new struct eventd_io_context;

    strcpy(ioc->addr, "127.0.0.1");
    ioc->port = 20530;

    ioc->buff_base = new char[1024];
    ioc->handle = handle;
    ioc->buff_size = 0;

    strcpy(ioc->buff_base, buff);
    ioc->buff_size = strlen(buff);

    ako_socket_send(ioc);

}

int ako_init(struct ako_callback_ops* ops) {
    int err = 0;
    /* �����ڴ���Դ */
    
    /* ע��һ����ʱ������1���Ӻ�ʼִ�У�ִֻ��һ�� */
    ako_timer_init(cron_timer_demo_fn, 1000, nullptr);
    ///* ע��һ����ʱ������3���Ӻ�ʼִ�У��ظ�600�� */
    ///ako_timer_repeat_init(cron_timer_demo2_fn, 10000, 3, nullptr);

    ako_event_callback_init(call_back_demo_fn);
    /* ����RMS�ڵ������ڴ���ping��Ϣ����ں���ָ�� */
    //ops->ping_fn = call_back_demo_fn;

    return err;
}

void ako_exit(void) {
    /* �ͷ��ڴ���Դ */
}
