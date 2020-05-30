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
typedef void (*ako_crontack_fn)(void);

/*
 * set a timer to perform specified segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 */
extern int ako_timer_init(ako_crontack_fn fn, uint64_t expire_ms);

/*
 * set a timer to perform specified repeat segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 * repeat: repeat times to run. set `repeat=0` to execute unlimited times
 */
extern int ako_timer_repeat_init(ako_crontack_fn fn, uint64_t expire_ms, uint64_t repeat);


struct eventd_config_t {
    int backlog;
    int port;
    char* addr;
};

struct eventd_socket_client {
    int backlog;
    int port;
    char* addr;
};

struct eventd_io_context {
    void *handle;
};

extern int ako_eventd_init(const struct eventd_config_t*);
extern void ako_eventd_exit();
extern void ako_eventd_loop();

extern struct eventd_io_context* ako_socket_client_new(struct eventd_socket_client*);
extern void ako_socket_client_free();
extern int ako_socket_send(struct eventd_io_context*, void* data, uint64_t size);



#ifdef __cplusplus
}
#endif

#endif // AKOFS_AKO_EVENTD_H_