/*
 * timer in C.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <ako_eventd.h>
#include "ako_eventd_core.h"

 /*
  * set a timer to perform specified segment
  * fn: task interface, enter for task thread
  * expire_ms: expire time to run the task, in millisecond
  */
int ako_timer_init(ako_crontack_fn fn, uint64_t expire_ms) {
    return 0;
}

/*
 * set a timer to perform specified repeat segment
 * fn: task interface, enter for task thread
 * expire_ms: expire time to run the task, in millisecond
 * repeat: repeat times to run. set `repeat=0` to execute unlimited times
 */
int ako_timer_repeat_init(ako_crontack_fn fn, uint64_t expire_ms, uint64_t repeat) {
    return 0;
}