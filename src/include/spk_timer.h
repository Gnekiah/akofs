/*
 * timer in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_TIMER_H_
#define SPARKLE_SPK_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * timer callback defination for task thread
 */
typedef void (*spk_timer_task_cb)(void);

/*
 * set a timer to perform specified segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 */
extern int spk_timer_init(spk_timer_task_cb fn, uint64_t expire_ms);

/*
 * set a timer to perform specified repeat segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 * repeat: repeat times to run. set `repeat=0` to execute unlimited times
 */
extern int spk_timer_repeat_init(spk_timer_task_cb fn, uint64_t expire_ms, uint64_t repeat);


#ifdef __cplusplus
}
#endif

#endif // SPARKLE_SPK_TIMER_H_