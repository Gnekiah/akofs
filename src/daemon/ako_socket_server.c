/*
 * Socket Server for CSS.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <ako_errno.h>
#include <assert.h>

#include "ako_eventd_core.h"

uv_tcp_t css_socket_server;

/* free socket client */
void css_socket_close_cb(uv_handle_t* client) {
    free(client);
}

/* free shutdown request */
void css_socket_shutdown_cb(uv_shutdown_t* req, int status) {
    uv_close((uv_handle_t*)req->handle, css_socket_close_cb);
    free(req);
}

void css_socket_alloc_cb(uv_handle_t* handle,
    size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

/* write and do clean */
void css_socket_write_cb(uv_write_t* req, int status) {
    uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
    int ret = uv_shutdown(shutdown_req, req->handle, css_socket_shutdown_cb);
}

void css_socket_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    int ret = 0;
    /* on read stage, the most important is to check `nread` */
    if (nread < 0) {
        if (buf->base)
            free(buf->base);

        uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
        ret = uv_shutdown(shutdown_req, stream, css_socket_shutdown_cb);
        assert(!ret);
        return;
    }

    if (nread == 0) {
        if (buf->base)
            free(buf->base);
        return;
    }

    uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));
    const char* resp = "Return 1";
    uv_buf_t new_buf = uv_buf_init((char*)resp, sizeof(resp) + 1);
    ret = uv_write(req, stream, &new_buf, 1, css_socket_write_cb);
}

/* socket server callback: init socket client and accept data */
static void css_connect_server_cb(uv_stream_t* server, int status) {
    int ret = 0;
    uv_tcp_t* tcp_client_handle = NULL;
    /* WARNING: alloc in heap, should be free() */
    tcp_client_handle = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    if (!tcp_client_handle) {
        return;
    }

    /* WARNING: call uv_close() on work end */
    ret = uv_tcp_init(server->loop, tcp_client_handle);
    if (ret) {
        return;
    }

    ret = uv_accept(server, (uv_stream_t*)tcp_client_handle);
    if (ret < 0) {
        /* if failed, do clean */
        uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
        if (!shutdown_req) {
            return;
        }
        ret = uv_shutdown(shutdown_req, (uv_stream_t*)tcp_client_handle,
            css_socket_shutdown_cb);
        return;
    }

    /* if uv_accept() is ok, do clean on read_cb */
    ret = uv_read_start((uv_stream_t*)tcp_client_handle,
        css_socket_alloc_cb, css_socket_read_cb);
}

int ako_server_init(const struct eventd_config_t* config) {
    int err = 0;
    int BACKLOG = config->backlog;
    struct sockaddr_in addr;

    if (!loop) {
        err = -AKOE_EVENTD_LOOP_NULL;
        goto out;
    }

    err = uv_tcp_init(loop, &css_socket_server);
    if (err) {
        err = -AKOE_EVENTD_TCP_INIT_ERROR;
        goto out;
    }

    err = uv_ip4_addr(config->addr, config->port, &addr);
    if (err) {
        err = -AKOE_EVENTD_ADDR_ERROR;
        goto err_addr;
    }

    err = uv_tcp_bind(&css_socket_server, (const struct sockaddr*) & addr, AF_UNSPEC);
    if (err) {
        err = -AKOE_EVENTD_TCP_BIND_ERROR;
        goto err_bind;
    }

    err = uv_listen((uv_stream_t*)& css_socket_server, BACKLOG,
        css_connect_server_cb);
    if (err) {
        err = -AKOE_EVENTD_TCP_LISTEN_ERROR;
        goto err_listen;
    }
    goto out;

err_listen:
err_bind:
err_addr:
    uv_close((uv_handle_t*)& css_socket_server, NULL);

out:
    return err;
}

void ako_server_exit() {
    uv_close((uv_handle_t*)& css_socket_server, NULL);

}
