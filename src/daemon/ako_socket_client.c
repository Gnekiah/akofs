/*
 * Socket Client impl in C
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
 /* for fix `error C2375`: redefinition errors in WinSock2.h */
#define WIN32_LEAN_AND_MEAN
#endif

#include "ako_eventd_core.h"
#include <ako_logger.h>
#include <assert.h>
#include<stdlib.h>

 /* free socket client */
void socket_close_cb(uv_handle_t* client) {
    free(client);
}

/* free shutdown request */
void socket_shutdown_cb(uv_shutdown_t* req, int status) {
    uv_close((uv_handle_t*)req->handle, socket_close_cb);
    free(req);
}

/*
void socket_alloc_cb(uv_handle_t* handle,
    size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}
*/

/* read callback for socket client */
/*
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
*/

static void socket_client_write_cb(uv_write_t* req, int status) {
    akolog_info("socket client write, status=%d", status);
    return;
}

 /* socket client after connected */
static void connect_client_cb(uv_connect_t* connect, int status) {
    int ret = 0;
    uv_stream_t* stream = connect->handle;

    return;
    
    /*
     * only C support this way
     * uv_buf_t buffer[] = {
     *     {
     *         .len  = 10,
     *         .base = (char*)"Command 1"
     *     }
     * };
     */

//    uv_buf_t buffer[1];
//    buffer[0].len = 10;
//    buffer[0].base = (char*)"Command 1";

//    uv_write_t request;
//    ret = uv_write(&request, stream, buffer, 1, socket_client_write_cb);

//     ret = uv_read_start(stream, socket_alloc_cb, socket_client_read_cb);
}

uv_connect_t* ako_socket_client_new(struct eventd_socket_client* conf) {
    int ret = 0;
    struct sockaddr_in addr;
    //uv_tcp_t* socket_client = NULL;
    uv_connect_t* connect = NULL;
//    struct eventd_io_context_t* context = NULL;
    uv_shutdown_t* shutdown_req = NULL;
    uv_tcp_t* socket_client = NULL;

//    context = (struct eventd_io_context_t*)malloc(sizeof(struct eventd_io_context));
//    if (!context) {
//        akolog_error("malloc `struct eventd_io_context_t` error, out of memory");
//        goto err_context;
//    }

    /* WARNING: do free after work end */
    akolog_info("sizeof(uv_tcp_t) is %lld", sizeof(uv_tcp_t));
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

    akolog_info("try to access socket_client->data is %lld", (uint64_t)socket_client->data);

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
        return connect;
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
//    free(context);
//    context = NULL;

err_context:
    return NULL;
}

void ako_socket_client_free() {
    return;
}

void _alloc_cb(uv_handle_t* handle,
    size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

static void _client_read_cb(uv_stream_t* tcp, ssize_t nread,
    const uv_buf_t* buf) {

    int ret = 0;

    if (nread > 0) {
        akolog_error("client read nread > 0");
    }
    else if (nread < 0) {
        akolog_error("client read nread < 0");
    }
    else if (nread == 0) {
        akolog_error("client read nread = 0");
    }

    uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
    ret = uv_shutdown(shutdown_req, tcp, socket_shutdown_cb);
    if (buf->base)
        free(buf->base);
}

int ako_socket_send(struct eventd_io_context* ioc) {
    int ret = 0;

    if (!ioc->handle) {
        struct eventd_socket_client client_conf;
        strcpy(client_conf.addr, ioc->addr);
        client_conf.port = ioc->port;
        client_conf.backlog = 0;
        uv_connect_t* conn = ako_socket_client_new(&client_conf);
        ioc->handle = (void*)(conn->handle);
        Sleep(1000);
    }

    uv_write_t *request = (uv_write_t*)malloc(sizeof(uv_write_t));
    uv_buf_t buffer = uv_buf_init(ioc->buff_base, ioc->buff_size);

    akolog_info("client start to send");
    ret = uv_write(request, ioc->handle, &buffer, 1, socket_client_write_cb);
    akolog_info("client sended");
    ret = uv_read_start(ioc->handle, _alloc_cb, _client_read_cb);
    return ret;
}