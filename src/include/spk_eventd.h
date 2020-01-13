/*
 * Sparkle Event Drive Module
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_EVENTD_H_
#define SPARKLE_SPK_EVENTD_H_

#ifdef __cplusplus
extern "C" {
#endif

struct eventd_config_t {
    struct {
        int backlog;
        int port;
        char* addr;
    } das_conf;
    struct {
        int backlog;
        int port;
        char* addr;
    } css_conf;
};

struct eventd_socket_client_t {
    int backlog;
    int port;
    char* addr;
};

struct eventd_io_context_t {
    void *handle;
};

extern int spk_eventd_init(const struct eventd_config_t*);
extern void spk_eventd_exit();
extern void spk_eventd_loop();

extern struct eventd_io_context_t* spk_socket_client_new(struct eventd_socket_client_t*);
extern void spk_socket_client_free();
extern int spk_socket_send(struct eventd_io_context_t*, void* data, uint64_t size);

#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_EVENTD_H_