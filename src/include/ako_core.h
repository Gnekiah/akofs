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
    void* conn_context;     /* point to eventd_io_context */
};

/*
 * eventd callback defination
 */
typedef void (ako_callback_fn)(struct ako_io_context*);

/*
 * callback defination ops
 */
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



//#ifdef __cplusplus
//extern "C" {
//#endif
/*
 * cron tack callback interface defination for task thread
 */
//typedef void (ako_crontack_fn)(void);
//
/*
 * set a timer to perform specified segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 */
//extern int ako_timer_init(ako_crontack_fn fn, uint64_t expire_ms);
//
/*
 * set a timer to perform specified repeat segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 * repeat: repeat times to run. set `repeat=0` to execute unlimited times
 */
//extern int ako_timer_repeat_init(ako_crontack_fn fn, uint64_t expire_ms, uint64_t repeat);
//#ifdef __cplusplus
//}
//#endif

extern int ako_rms_init(struct ako_rms_ops* rms);
extern void ako_rms_exit(void);
extern int ako_css_init(struct ako_css_ops* css);
extern void ako_css_exit(void);
extern int ako_das_init(struct ako_das_ops* das);
extern void ako_das_exit(void);


#endif // AKOFS_AKO_CORE_H_