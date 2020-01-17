/*
 * Sparkle low-level core module.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_CORE_H_
#define SPARKLE_SPK_CORE_H_

#include <stdint.h>
#include <mutex>
#include <message/Message.hpp>

/*
 * IO context
 */
struct spk_io_context {
    Message* message;
    std::mutex lock;
    void* conn_context;     /* point to eventd_io_context */
};

/*
 * eventd callback defination
 */
typedef void (spk_callback_fn)(struct spk_io_context*);

/*
 * ops
 */
struct spk_callback_ops {
    spk_callback_fn ping_fn;
    spk_callback_fn pong_fn;
    spk_callback_fn req_probe_fn;
    spk_callback_fn resp_probe_fn;
    spk_callback_fn req_join_fn;
    spk_callback_fn resp_join_fn;

    spk_callback_fn req_sync_fn;
    spk_callback_fn resp_sync_fn;

    spk_callback_fn req_sub_sync_fn;
    spk_callback_fn resp_sub_sync_fn;
    spk_callback_fn rtn_sync_fn;

    spk_callback_fn req_unsub_sync_fn;
    spk_callback_fn resp_unsub_sync_fn;
    spk_callback_fn ntc_sync_fn;

    spk_callback_fn req_get_metadata_fn;
    spk_callback_fn resp_get_metadata_fn;
    spk_callback_fn req_put_metadata_fn;
    spk_callback_fn resp_put_metadata_fn;

    spk_callback_fn req_get_blk_mapping_fn;
    spk_callback_fn resp_get_blk_mapping_fn;
    spk_callback_fn req_put_blk_mapping_fn;
    spk_callback_fn resp_put_blk_mapping_fn;

    spk_callback_fn req_get_chunk_fn;
    spk_callback_fn resp_get_chunk_fn;
    spk_callback_fn req_put_chunk_fn;
    spk_callback_fn resp_put_chunk_fn;

    spk_callback_fn req_fsck_fn;
    spk_callback_fn resp_fsck_fn;

    spk_callback_fn cmd_shutdown_fn;
    spk_callback_fn resp_cmd_shutdown_fn;
    spk_callback_fn cmd_reboot_fn;
    spk_callback_fn resp_cmd_reboot_fn;
};

/*
 * Regional Manager callback defination
 */
struct spk_rms_ops {
    uint64_t rms_id;
    struct spk_callback_ops ops;
};

/*
 * Chunk Storage Subject callback defination
 */
struct spk_css_ops {
    uint64_t css_id;
    struct spk_callback_ops ops;
};

/*
 * Data Access Subject callback defination
 */
struct spk_das_ops {
    uint64_t das_id;
    struct spk_callback_ops ops;
};

//#ifdef __cplusplus
//extern "C" {
//#endif
/*
 * cron tack callback interface defination for task thread
 */
//typedef void (spk_crontack_fn)(void);
//
/*
 * set a timer to perform specified segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 */
//extern int spk_timer_init(spk_crontack_fn fn, uint64_t expire_ms);
//
/*
 * set a timer to perform specified repeat segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 * repeat: repeat times to run. set `repeat=0` to execute unlimited times
 */
//extern int spk_timer_repeat_init(spk_crontack_fn fn, uint64_t expire_ms, uint64_t repeat);
//#ifdef __cplusplus
//}
//#endif

extern int spk_rms_init(struct spk_rms_ops* rms);
extern void spk_rms_exit(void);
extern int spk_css_init(struct spk_css_ops* css);
extern void spk_css_exit(void);
extern int spk_das_init(struct spk_das_ops* das);
extern void spk_das_exit(void);


#endif // SPARKLE_SPK_CORE_H_