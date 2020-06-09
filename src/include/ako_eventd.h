/*
 * AkoFS Event Drive Module
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_AKO_EVENTD_H_
#define AKOFS_AKO_EVENTD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * cron tack callback interface defination for task thread
 */
typedef void (*ako_crontack_fn)(void*);
/*
 * callback interface defination for response
 */
typedef void (*ako_callback_fn)(void* handle, uint64_t nread, 
    uint64_t buff_size, char* buff_base);
/*
 * set a timer to perform specified segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 */
extern int ako_timer_init(ako_crontack_fn fn, uint64_t expire_ms, 
    void* handle);
/*
 * set a timer to perform specified repeat segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 * repeat: repeat times to run. set `repeat=0` to execute unlimited times
 */
extern int ako_timer_repeat_init(ako_crontack_fn fn, uint64_t expire_ms, 
    uint64_t repeat, void* handle);
/*
 * set a callback for request
 * fn: request interface
 */
extern int ako_event_callback_init(ako_callback_fn fn);

struct eventd_config_t {
    int backlog;
    int port;
    char addr[16];
};

struct eventd_socket_client {
    int backlog;
    int port;
    char addr[16];
};

struct eventd_io_context {
    void *handle;
    char* buff_base;
    uint64_t buff_size;
    char addr[16];
    int  port;
};

extern int ako_eventd_init(const struct eventd_config_t*);
extern void ako_eventd_exit();
extern void ako_eventd_loop();

//extern struct eventd_io_context* ako_socket_client_new(struct eventd_socket_client*);
//extern void ako_socket_client_free();
extern int ako_socket_send(struct eventd_io_context* ioc);

#ifdef __cplusplus
}
#endif

#endif // AKOFS_AKO_EVENTD_H_