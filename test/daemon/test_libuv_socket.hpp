#ifndef AKOFS_TEST_LIBUV_SOCKET_HPP_
#define AKOFS_TEST_LIBUV_SOCKET_HPP_

#include<gtest/gtest.h>
#include<cstdlib>
#include<uv.h>

/************************************************
 * A LNK2019 error was fixed caused by x86_32 lib
 * de, miku chan no tasukete no moto ni.
 * miku chan, arigadou gozai masu!     (*^_^*)~~~
 *_________________#####_________________________
 *_______________#########_______________________
 *______________############_____________________
 *______________#############____________________
 *_____________##__###########___________________
 *____________###__######_#####__________________
 *____________###_#######___####_________________
 *___________###__##########_####________________
 *__________####__###########_####_______________
 *________#####___###########__#####_____________
 *_______######___###_########___#####___________
 *_______#####___###___########___######_________
 *______######___###__###########___######_______
 *_____######___####_##############__######______
 *____#######__#####################_#######_____
 *____#######__##############################____
 *___#######__######_#################_#######___
 *___#######__######_######_#########___######___
 *___#######____##__######___######_____######___
 *___#######________######____#####_____#####____
 *____######________#####_____#####_____####_____
 *_____#####________####______#####_____###______
 *______#####______;###________###;_____#________
 *________##_______####________####______________
 ************************************************/

/************************************************/
/*
 * IMPORTANT: Fully close a loop!!!
 * see alse at github.com/joyent/libuv/blob/master/test/task.h#L221-L231
 */
static void test_libuv_close_walk_cb(uv_handle_t* handle, void* arg) {
    if (!uv_is_closing(handle))
        uv_close(handle, NULL);
}

static void test_libuv_close_loop(uv_loop_t* loop) {
    uv_walk(loop, test_libuv_close_walk_cb, NULL);
    uv_run(loop, UV_RUN_DEFAULT);
}

/************************************************/
TEST(test_libuv_loop, case_event_loop_test_1) {
    int ret = 0;
    uv_loop_t* loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    EXPECT_TRUE(loop);
    ret = uv_loop_init(loop);
    EXPECT_FALSE(ret);

    ret = uv_run(loop, UV_RUN_DEFAULT);
    EXPECT_FALSE(ret);
    ret = uv_loop_close(loop);
    EXPECT_FALSE(ret);

    free(loop);
}

/************************************************/
int64_t test_libuv_loop_counter = 0;
void test_libuv_loop_wait_for_a_while(uv_idle_t* handle) {
    test_libuv_loop_counter++;
    if (test_libuv_loop_counter >= 1000000) {
        int ret = uv_idle_stop(handle);
        EXPECT_FALSE(ret);
    }
}

TEST(test_libuv_loop, case_event_loop_idle_basic_test_1) {
    int ret = 0;
    uv_idle_t idler;
    /* WARNING: This function is not thread safe. */
    uv_loop_t* loop = uv_default_loop();

    ret = uv_idle_init(loop, &idler);
    EXPECT_FALSE(ret);
    ret = uv_idle_start(&idler, test_libuv_loop_wait_for_a_while);
    EXPECT_FALSE(ret);

    ret = uv_run(loop, UV_RUN_DEFAULT);
    EXPECT_FALSE(ret);
    EXPECT_EQ(1000000, test_libuv_loop_counter);

    /* IMPORTANT: Fully close a loop!!! */
    test_libuv_close_loop(loop);
    EXPECT_FALSE(uv_is_closing((uv_handle_t*)loop));
    ret = uv_loop_close(loop);
    EXPECT_FALSE(ret);
}

/************************************************/
static int test_libuv_socket_function_flag_check[12] = { 0 };

void test_libuv_socket_alloc_cb(uv_handle_t* handle,
    size_t suggested_size, uv_buf_t* buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;

    /* flagset for checking */
    test_libuv_socket_function_flag_check[9]++;
}

/* free socket client */
void test_libuv_socket_close_cb(uv_handle_t* client) {
    free(client);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[8]++;
}

/* free shutdown request */
void test_libuv_socket_shutdown_cb(uv_shutdown_t* req, int status) {
    EXPECT_EQ(0, status);

    uv_close((uv_handle_t*)req->handle, test_libuv_socket_close_cb);
    free(req);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[7]++;
}

/* write and do clean */
void test_libuv_socket_write_cb(uv_write_t* req, int status) {
    EXPECT_EQ(0, status);

    uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
    int ret = uv_shutdown(shutdown_req, req->handle, test_libuv_socket_shutdown_cb);
    EXPECT_EQ(0, ret);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[6]++;
}

/* read data */
void test_libuv_socket_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    int ret = 0;
    /* on read stage, the most important is to check `nread` */
    if (nread < 0) {
        EXPECT_EQ(UV_EOF, nread);
        if (buf->base)
            free(buf->base);

        uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
        ret = uv_shutdown(shutdown_req, stream, test_libuv_socket_shutdown_cb);
        EXPECT_EQ(0, ret);
        return;
    }

    if (nread == 0) {
        if (buf->base)
            free(buf->base);
        return;
    }

    auto test_libuv_socket_write_to_client = [](const char* resp, uv_stream_t* stream) {
        int ret = 0;
        uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));

        uv_buf_t buf = uv_buf_init((char*)resp, sizeof(resp)+1);
        ret = uv_write(req, stream, &buf, 1, test_libuv_socket_write_cb);
    };

    // read data
    if (!strcmp("Command 1", buf->base)) {
        test_libuv_socket_write_to_client("Return 1", stream);
    }
    else if (!strcmp("Command 2", buf->base)) {
        test_libuv_socket_write_to_client("Return 12", stream);
    }
    else {
        test_libuv_socket_write_to_client("Unknown Command", stream);
    }

    /* flagset for checking */
    test_libuv_socket_function_flag_check[5]++;
}

/* socket server callback: init socket client and accept data */
void test_libuv_socket_connect_server_cb(uv_stream_t* server, int status) {
    EXPECT_EQ(0, status);

    int ret = 0;
    /* WARNING: alloc in heap, should be free() */
    uv_tcp_t* tcp_client_handle = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    /* WARNING: call uv_close() on work end */
    ret = uv_tcp_init(server->loop, tcp_client_handle);
    EXPECT_EQ(0, ret);

    ret = uv_accept(server, (uv_stream_t*)tcp_client_handle);
    EXPECT_EQ(0, ret);

    if (ret < 0) {
        /* if failed, do clean */
        uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
        ret = uv_shutdown(shutdown_req, (uv_stream_t*)tcp_client_handle, 
            test_libuv_socket_shutdown_cb);
        EXPECT_EQ(0, ret);
        return;
    }

    /* if uv_accept() is ok, do clean on read_cb */
    ret = uv_read_start((uv_stream_t*)tcp_client_handle, 
        test_libuv_socket_alloc_cb, test_libuv_socket_read_cb);
    EXPECT_EQ(0, ret);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[4]++;
}

/* read callback for socket client */
static void test_libuv_socket_client_read_cb(uv_stream_t* tcp, ssize_t nread, 
    const uv_buf_t* buf) {
    
    int ret = 0;

    if (nread > 0) {
        if (nread == 9)
            EXPECT_STREQ("Return 1", buf->base);
        else if (nread == 10)
            EXPECT_STREQ("Return 12", buf->base);
        else
            EXPECT_STREQ("Unknown Command", buf->base);
    }
    else if (nread < 0) {
        EXPECT_EQ(UV_EOF, nread);
    }
    else if (nread == 0) {
        EXPECT_TRUE(0);
    }

    uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
    ret = uv_shutdown(shutdown_req, tcp, test_libuv_socket_shutdown_cb);
    if (buf->base)
        free(buf->base);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[3]++;
}

static void test_libuv_socket_client_write_cb(uv_write_t* req, int status) {
    EXPECT_EQ(0, status);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[0]++;
}

/* socket client after connected */
static void test_libuv_socket_connect_client_cb(uv_connect_t* connect, int status) {
    EXPECT_EQ(0, status);

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
    ret = uv_write(&request, stream, buffer, 1, test_libuv_socket_client_write_cb);
    EXPECT_EQ(0, ret);

    ret = uv_read_start(stream, test_libuv_socket_alloc_cb, test_libuv_socket_client_read_cb);
    EXPECT_EQ(0, ret);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[2]++;
}

void test_libuv_socket_timer_socket_client_cb(uv_timer_t* handle) {
    struct sockaddr_in addr;
    uv_loop_t* loop = uv_default_loop();
    int ret = 0;

    /* WARNING: do free after work end */
    uv_tcp_t* test_libuv_socket_client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

    //akolog_hex_info(test_libuv_socket_client, sizeof(test_libuv_socket_client));

    ret = uv_ip4_addr("127.0.0.1", 37551, &addr);
    EXPECT_EQ(0, ret);
    /* WARNING: call uv_close() on work end */
    ret = uv_tcp_init(loop, test_libuv_socket_client);
    EXPECT_EQ(0, ret);
    
    ret = uv_tcp_connect(connect, test_libuv_socket_client, 
        (const struct sockaddr*) & addr, test_libuv_socket_connect_client_cb);
    EXPECT_EQ(0, ret);

    if (ret) {
        /* letdown to be reached */
        EXPECT_TRUE(0);
        uv_shutdown_t* shutdown_req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
        ret = uv_shutdown(shutdown_req, (uv_stream_t*)test_libuv_socket_client, 
            test_libuv_socket_shutdown_cb);
        EXPECT_EQ(0, ret);
    }

    ret = uv_timer_stop(handle);
    EXPECT_EQ(0, ret);
    ret = uv_loop_alive(handle->loop);
    EXPECT_EQ(1, ret);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[1]++;
}

uv_tcp_t test_libuv_socket_server;
uv_async_t test_libuv_socket_async_handle;

void test_libuv_socket_async_close_loop_cb(uv_async_t* handle) {
    uv_close((uv_handle_t*)& test_libuv_socket_server, NULL);
    uv_close((uv_handle_t*)handle, NULL);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[11]++;
}

void test_libuv_socket_timer_close_loop_cb(uv_timer_t* handle) {
    int ret = 0;
    uint64_t tmr = 0;
    uv_loop_t* loop = uv_default_loop();

    tmr = uv_now(handle->loop);
    EXPECT_LT(0, tmr);
    tmr = uv_hrtime();
    EXPECT_LT(0, tmr);

    /* flagset for checking */
    test_libuv_socket_function_flag_check[10]++;

    ret = uv_async_init(loop, &test_libuv_socket_async_handle, 
        test_libuv_socket_async_close_loop_cb);
    EXPECT_EQ(0, ret);

    ret = uv_async_send(&test_libuv_socket_async_handle);
    EXPECT_EQ(0, ret);

    ret = uv_timer_stop(handle);
    EXPECT_EQ(0, ret);
    ret = uv_loop_alive(handle->loop);
    EXPECT_EQ(1, ret);
}

TEST(test_libuv_socket, case_socket_tcp_server_test_1) {
    uv_loop_t* loop = uv_default_loop();
    const int BACKLOG = 128;
    struct sockaddr_in addr;
    uv_timer_t timer_socket_client, timer_close_loop;
    int ret = 0;

    /* socket server init and listen for accepting */
    ret = uv_tcp_init(loop, &test_libuv_socket_server);
    EXPECT_EQ(0, ret);
    ret = uv_ip4_addr("0.0.0.0", 37551, &addr);
    EXPECT_EQ(0, ret);
    ret = uv_tcp_bind(&test_libuv_socket_server, (const struct sockaddr*) &addr, AF_UNSPEC);
    EXPECT_EQ(0, ret);
    ret = uv_listen((uv_stream_t*)& test_libuv_socket_server, BACKLOG,
        test_libuv_socket_connect_server_cb);
    EXPECT_EQ(0, ret);

    /* timer init */
    ret = uv_timer_init(loop, &timer_socket_client);
    EXPECT_EQ(0, ret);
    ret = uv_timer_start(&timer_socket_client, 
        test_libuv_socket_timer_socket_client_cb, 10, 0);
    EXPECT_EQ(0, ret);

    ret = uv_timer_init(loop, &timer_close_loop);
    EXPECT_EQ(0, ret);
    ret = uv_timer_start(&timer_close_loop, 
        test_libuv_socket_timer_close_loop_cb, 1000, 0);
    EXPECT_EQ(0, ret);

    /* loop */
    /* WARNING: its thread unsafe!!!! */
    ret = uv_run(loop, UV_RUN_DEFAULT);
    EXPECT_EQ(0, ret);

    /* close loop */
    test_libuv_close_loop(loop);
    ret = uv_is_closing((uv_handle_t*)loop);
    EXPECT_EQ(0, ret);
    ret = uv_loop_close(loop);
    EXPECT_EQ(0, ret);

    /* test_libuv_socket_client_write_cb */
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[0]);
    /* test_libuv_socket_timer_socket_client_cb */
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[1]);
    /* test_libuv_socket_connect_client_cb */
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[2]);
    /* test_libuv_socket_client_read_cb */
#ifdef __linux__
    EXPECT_EQ(2, test_libuv_socket_function_flag_check[3]);
#else
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[3]);
#endif 
    /* test_libuv_socket_connect_server_cb */
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[4]);
    /* test_libuv_socket_read_cb */
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[5]);
    /* test_libuv_socket_write_cb */
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[6]);
    /* test_libuv_socket_shutdown_cb */
    EXPECT_EQ(2, test_libuv_socket_function_flag_check[7]);
    /* test_libuv_socket_close_cb */
    EXPECT_EQ(2, test_libuv_socket_function_flag_check[8]);
    /* test_libuv_socket_alloc_cb */
#ifdef __linux__ 
    EXPECT_EQ(3, test_libuv_socket_function_flag_check[9]);
#else 
    EXPECT_EQ(2, test_libuv_socket_function_flag_check[9]);
#endif 
    /* test_libuv_socket_timer_close_loop_cb */
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[10]);
    /* test_libuv_socket_async_close_loop_cb */
    EXPECT_EQ(1, test_libuv_socket_function_flag_check[11]);
}

#endif // AKOFS_TEST_LIBUV_SOCKET_HPP_
