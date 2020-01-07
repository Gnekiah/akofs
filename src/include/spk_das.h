/*
 * Sparkle Data Access Subject Interface
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_DAS_H_
#define SPARKLE_SPK_DAS_H_

#include <spk_core.h>

extern int spk_das_init(struct spk_das_ops* das);

extern void spk_das_exit(void);

#endif // SPARKLE_SPK_DAS_H_