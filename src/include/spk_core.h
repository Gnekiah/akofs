/*
 * Sparkle low-level core module.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_CORE_H_
#define SPARKLE_SPK_CORE_H_

#include <stdint.h>

typedef void (rms_)();

struct spk_rms_ops {
    uint64_t rms_id;
};

struct spk_css_ops {
    uint64_t css_id;
};

struct spk_das_ops {
    uint64_t das_id;
};


typedef int (nvm_l2p_update_fn)(u64, u32, __le64*, void*);
typedef int (nvm_id_fn)(struct nvm_dev*, struct nvm_id*);
typedef int (nvm_get_l2p_tbl_fn)(struct nvm_dev*, u64, u32,
    nvm_l2p_update_fn*, void*);
typedef int (nvm_op_bb_tbl_fn)(struct nvm_dev*, struct ppa_addr, u8*);
typedef int (nvm_op_set_bb_fn)(struct nvm_dev*, struct ppa_addr*, int, int);
typedef int (nvm_submit_io_fn)(struct nvm_dev*, struct nvm_rq*);
typedef void* (nvm_create_dma_pool_fn)(struct nvm_dev*, char*);
typedef void (nvm_destroy_dma_pool_fn)(void*);
typedef void* (nvm_dev_dma_alloc_fn)(struct nvm_dev*, void*, gfp_t,
    dma_addr_t*);
typedef void (nvm_dev_dma_free_fn)(void*, void*, dma_addr_t);

struct nvm_dev_ops {
    nvm_id_fn* identity;
    nvm_get_l2p_tbl_fn* get_l2p_tbl;
    nvm_op_bb_tbl_fn* get_bb_tbl;
    nvm_op_set_bb_fn* set_bb_tbl;

    nvm_submit_io_fn* submit_io;

    nvm_create_dma_pool_fn* create_dma_pool;
    nvm_destroy_dma_pool_fn* destroy_dma_pool;
    nvm_dev_dma_alloc_fn* dev_dma_alloc;
    nvm_dev_dma_free_fn* dev_dma_free;

    unsigned int		max_phys_sect;
};


#endif // SPARKLE_SPK_CORE_H_