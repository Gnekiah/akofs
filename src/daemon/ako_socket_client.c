/*
 * Socket Client impl in C
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include "ako_eventd_core.h"
#include <ako_logger.h>
#include <assert.h>

 /* free socket client */
void socket_close_cb(uv_handle_t* client) {
    free(client);
}

/* free shutdown request */
void socket_shutdown_cb(uv_shutdown_t* req, int status) {
    uv_close((uv_handle_t*)req->handle, socket_close_cb);
    free(req);
}

void socket_alloc_cb(uv_handle_t* handle,
    size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

/* read callback for socket client */
static void socket_client_read_cb(uv_stream_t* tcp, ssize_t nread,
    const uv_buf_t* buf) {

    int ret = 0;

    if (nread > 0) {
        if (nread == 9)
            //EXPECT_STREQ("Return 1", buf->base);
            ;
        else if (nread == 10)
            //EXPECT_STREQ("Return 12", buf->base);
            ;
        else
            //EXPECT_STREQ("Unknown Command", buf->base);
            ;
    }
    else if (nread < 0) {
        ;
        //EXPECT_EQ(UV_EOF, nread);
    }
    else if (nread == 0) {
        //EXPECT_TRUE(0);
        ;
    }

    uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
    ret = uv_shutdown(shutdown_req, tcp, socket_shutdown_cb);
    if (buf->base)
        free(buf->base);
}

static void socket_client_write_cb(uv_write_t* req, int status) {
    
}

 /* socket client after connected */
static void connect_client_cb(uv_connect_t* connect, int status) {
    int ret = 0;
    uv_stream_t* stream = connect->handle;

    /*
     * only C support this way
     * uv_buf_t buffer[] = {
     *     {
     *         .len  = 10,
     *         .base = (char*)"Command 1"
     *     }
     * };
     */

    uv_buf_t buffer[1];
    buffer[0].len = 10;
    buffer[0].base = (char*)"Command 1";

    uv_write_t request;
    ret = uv_write(&request, stream, buffer, 1, socket_client_write_cb);

    ret = uv_read_start(stream, socket_alloc_cb, socket_client_read_cb);
}

struct eventd_io_context* ako_socket_client_new(struct 
    eventd_socket_client* conf) {
    int ret = 0;
    struct sockaddr_in addr;
    uv_tcp_t* socket_client = NULL;
    uv_connect_t* connect = NULL;
    struct eventd_io_context_t* context = NULL;
    uv_shutdown_t* shutdown_req = NULL;

    context = (struct eventd_io_context_t*)malloc(sizeof(struct eventd_io_context));
    if (!context) {
        akolog_error("malloc `struct eventd_io_context_t` error, out of memory");
        goto err_context;
    }

    /* WARNING: do free after work end */
    socket_client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
    if (!connect || !socket_client) {
        akolog_error("malloc `uv_tcp_t/uv_connect_t` error, out of memory");
        goto err_alloc;
    }

    ret = uv_ip4_addr(conf->addr, conf->port, &addr);
    if (ret) {
        akolog_error("malloc `uv_tcp_t/uv_connect_t` error, out of memory");
        goto err_addr_parse;
    }

    /* WARNING: call uv_close() on work end */
    ret = uv_tcp_init(loop, socket_client);
    if (ret) {
        akolog_error("tcp init error, error code = ", ret);
        goto err_addr_parse;
    }

    ret = uv_tcp_connect(connect, socket_client,
        (const struct sockaddr*) & addr, connect_client_cb);

    if (!ret) {
        akolog_debug("new socket client allocated");
        return context;
    }

    /* letdown to be reached */
    shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
    ret = uv_shutdown(shutdown_req, (uv_stream_t*)socket_client,
        socket_shutdown_cb);

err_addr_parse:
    free(socket_client);
    free(connect);
    socket_client = NULL;
    connect = NULL;

err_alloc:
    free(context);
    context = NULL;

err_context:
    return NULL;
}