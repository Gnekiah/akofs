/*
 * Sparkle low-level core module.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_CORE_H_
#define SPARKLE_SPK_CORE_H_

#include <stdint.h>

typedef void (rms_)(struct nvm_dev*, struct nvm_id*);

struct spk_rms_ops {
    uint64_t rms_id;
};

struct spk_css_ops {
    uint64_t css_id;
};

struct spk_das_ops {
    uint64_t das_id;
};

extern int spk_rms_init(struct spk_rms_ops* rms);
extern void spk_rms_exit(void);
extern int spk_css_init(struct spk_css_ops* css);
extern void spk_css_exit(void);
extern int spk_das_init(struct spk_das_ops* das);
extern void spk_das_exit(void);


#endif // SPARKLE_SPK_CORE_H_