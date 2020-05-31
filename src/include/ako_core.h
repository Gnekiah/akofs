/*
 * AkoFS low-level core module.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_AKO_CORE_H_
#define AKOFS_AKO_CORE_H_

#include <stdint.h>
#include <mutex>
#include <message/Message.hpp>

/*
 * IO context
 */
struct ako_io_context {
    Message* message;
    std::mutex lock;
    char* buff_base;
    uint64_t buff_size;
    void* conn_context;     /* point to eventd_io_context */
};

/*
 * eventd callback defination
 */
//typedef void (*ako_callback_fn)(struct ako_io_context*);

/*
 * callback defination ops
 */
/*
struct ako_callback_ops {
    ako_callback_fn ping_fn;
    ako_callback_fn pong_fn;
    ako_callback_fn req_probe_fn;
    ako_callback_fn resp_probe_fn;
    ako_callback_fn req_join_fn;
    ako_callback_fn resp_join_fn;

    ako_callback_fn req_sync_fn;
    ako_callback_fn resp_sync_fn;

    ako_callback_fn req_sub_sync_fn;
    ako_callback_fn resp_sub_sync_fn;
    ako_callback_fn rtn_sync_fn;

    ako_callback_fn req_unsub_sync_fn;
    ako_callback_fn resp_unsub_sync_fn;
    ako_callback_fn ntc_sync_fn;

    ako_callback_fn req_get_metadata_fn;
    ako_callback_fn resp_get_metadata_fn;
    ako_callback_fn req_put_metadata_fn;
    ako_callback_fn resp_put_metadata_fn;

    ako_callback_fn req_get_blk_mapping_fn;
    ako_callback_fn resp_get_blk_mapping_fn;
    ako_callback_fn req_put_blk_mapping_fn;
    ako_callback_fn resp_put_blk_mapping_fn;

    ako_callback_fn req_get_chunk_fn;
    ako_callback_fn resp_get_chunk_fn;
    ako_callback_fn req_put_chunk_fn;
    ako_callback_fn resp_put_chunk_fn;

    ako_callback_fn req_fsck_fn;
    ako_callback_fn resp_fsck_fn;

    ako_callback_fn cmd_shutdown_fn;
    ako_callback_fn resp_cmd_shutdown_fn;
    ako_callback_fn cmd_reboot_fn;
    ako_callback_fn resp_cmd_reboot_fn;
};
*/
struct ako_callback_ops {
    int i;
};

extern int ako_init(struct ako_callback_ops* ops);
extern void ako_exit(void);


#endif // AKOFS_AKO_CORE_H_