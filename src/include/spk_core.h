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
    void* conn_context;
};

/*
 * eventd callback defination
 */
typedef void (spk_callback_fn)(struct spk_io_context*);

/*
 * Regional Manager callback defination
 */
struct spk_rms_ops {
    uint64_t rms_id;
};

/*
 * Chunk Storage Subject callback defination
 */
struct spk_css_ops {
    uint64_t css_id;
};

/*
 * Data Access Subject callback defination
 */
struct spk_das_ops {
    uint64_t das_id;
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