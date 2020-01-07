/*
 * Sparkle Regional Manager Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_RMS_H_
#define SPARKLE_SPK_RMS_H_

#include <spk_core.h>

extern int spk_rms_init(struct spk_rms_ops* rms);

extern void spk_rms_exit(void);

#endif // SPARKLE_SPK_RMS_H_