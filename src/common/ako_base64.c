/*
 * ako_base64.c -- ako_base64_encode() and ako_base64_decode()
 * based on libcrypto
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <ako_compto.h>

#define BASE64_ENCODE_OUT_SIZE(s) ((unsigned int)((((s) + 2) / 3) * 4 + 1))
#define BASE64_DECODE_OUT_SIZE(s) ((unsigned int)(((s) / 4) * 3))
#define BASE64_PAD '='
#define BASE64DE_FIRST '+'
#define BASE64DE_LAST 'z'

/* BASE 64 encode table */
static const int8_t base64en[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};

/* ASCII order for BASE 64 decode, 255 in unused character */
static const uint8_t base64de[] = {
    /* nul, soh, stx, etx, eot, enq, ack, bel, */
    255, 255, 255, 255, 255, 255, 255, 255,
    /*  bs,  ht,  nl,  vt,  np,  cr,  so,  si, */
    255, 255, 255, 255, 255, 255, 255, 255,
    /* dle, dc1, dc2, dc3, dc4, nak, syn, etb, */
    255, 255, 255, 255, 255, 255, 255, 255,
    /* can,  em, sub, esc,  fs,  gs,  rs,  us, */
    255, 255, 255, 255, 255, 255, 255, 255,
    /*  sp, '!', '"', '#', '$', '%', '&', ''', */
    255, 255, 255, 255, 255, 255, 255, 255,
    /* '(', ')', '*', '+', ',', '-', '.', '/', */
    255, 255, 255,  62, 255, 255, 255,  63,
    /* '0', '1', '2', '3', '4', '5', '6', '7', */
    52,  53,  54,  55,  56,  57,  58,  59,
    /* '8', '9', ':', ';', '<', '=', '>', '?', */
    60,  61, 255, 255, 255, 255, 255, 255,
    /* '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', */
    255,   0,   1,  2,   3,   4,   5,    6,
    /* 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', */
    7,   8,   9,  10,  11,  12,  13,  14,
    /* 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', */
    15,  16,  17,  18,  19,  20,  21,  22,
    /* 'X', 'Y', 'Z', '[', '\', ']', '^', '_', */
    23,  24,  25, 255, 255, 255, 255, 255,
    /* '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', */
    255,  26,  27,  28,  29,  30,  31,  32,
    /* 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', */
    33,  34,  35,  36,  37,  38,  39,  40,
    /* 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', */
    41,  42,  43,  44,  45,  46,  47,  48,
    /* 'x', 'y', 'z', '{', '|', '}', '~', del, */
    49,  50,  51, 255, 255, 255, 255, 255
};

uint64_t ako_base64_encode(const uint8_t* src, uint64_t src_size, int8_t* dst) {
    int state = 0;
    uint64_t i = 0, j = 0;
    uint8_t curr, prev = 0;

    for (i = j = 0; i < src_size; i++) {
        curr = src[i];
        switch (state) {
        case 0:
            state = 1;
            dst[j++] = base64en[(curr >> 2) & 0x3F];
            break;
        case 1:
            state = 2;
            dst[j++] = base64en[((prev & 0x3) << 4) | ((curr >> 4) & 0xF)];
            break;
        case 2:
            state = 0;
            dst[j++] = base64en[((prev & 0xF) << 2) | ((curr >> 6) & 0x3)];
            dst[j++] = base64en[curr & 0x3F];
            break;
        }
        prev = curr;
    }

    switch (state) {
    case 1:
        dst[j++] = base64en[(prev & 0x3) << 4];
        dst[j++] = BASE64_PAD;
        dst[j++] = BASE64_PAD;
        break;
    case 2:
        dst[j++] = base64en[(prev & 0xF) << 2];
        dst[j++] = BASE64_PAD;
        break;
    }

    dst[j] = 0;
    return j;
}

uint64_t ako_base64_decode(const int8_t* src, uint64_t src_size, uint8_t* dst) {
    uint64_t i = 0, j = 0;

    if (src_size & 0x3) {
        return 0;
    }

    for (i = j = 0; i < src_size; i++) {
        if (src[i] == BASE64_PAD) {
            break;
        }
        if (src[i] < BASE64DE_FIRST || src[i] > BASE64DE_LAST) {
            return 0;
        }
        uint8_t curr = base64de[(unsigned char)src[i]];
        if (curr == 255) {
            return 0;
        }

        switch (i & 0x3) {
        case 0:
            dst[j] = (curr << 2) & 0xFF;
            break;
        case 1:
            dst[j++] |= (curr >> 4) & 0x3;
            dst[j] = (curr & 0xF) << 4;
            break;
        case 2:
            dst[j++] |= (curr >> 2) & 0xF;
            dst[j] = (curr & 0x3) << 6;
            break;
        case 3:
            dst[j++] |= curr;
            break;
        }
    }

    return j;
}

inline uint64_t ako_base64_enbound(uint64_t size) {
    return (size + 2) / 3 * 4;
}

inline uint64_t ako_base64_debound(uint64_t size) {
    return size / 4 * 3;
}
