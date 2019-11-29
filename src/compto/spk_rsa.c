/*
 * spk_rsa.c
 * based on libcrypto
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <spk_compto.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

int spk_rsa_generate_keypair_2048(int8_t* pri_key, uint64_t* pri_key_size,
    int8_t* pub_key, uint64_t* pub_key_size) {
    int ret = 0;
    RSA* keypair = RSA_new();
    BIGNUM* bgn = BN_new();
    BIO* pri = BIO_new(BIO_s_mem());
    BIO* pub = BIO_new(BIO_s_mem());
    ret = BN_set_word(bgn, RSA_F4);
    // wtf! its return 1 while done ok, else return -1
    ret = RSA_generate_key_ex(keypair, 2048, bgn, NULL);
    if (ret != 1) {
        ret = -1;
        goto free;
    }
    
    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, keypair);
    
    uint64_t pri_len = BIO_pending(pri);
    uint64_t pub_len = BIO_pending(pub);

    if (*pri_key_size <= pri_len || *pub_key_size <= pub_len) {
        ret = -1;
        goto free;
    }

    *pri_key_size = pri_len;
    *pub_key_size = pub_len;

    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);
    pri_key[pri_len] = 0;
    pub_key[pub_len] = 0;

    ret = 0;

free:
    BIO_free_all(pri);
    BIO_free_all(pub);
    BN_free(bgn);
    RSA_free(keypair);
    return ret;
}

/* WARNING: src_size can NOT bigger than 245 bytes. */
uint64_t spk_rsa_pub_encrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    uint64_t dst_size, const int8_t* pub_key) {
    int ret = 0;
    BIO* pub = BIO_new_mem_buf(pub_key, -1);
    RSA* rsa = NULL;
    rsa = PEM_read_bio_RSAPublicKey(pub, &rsa, NULL, NULL);

    int len = RSA_size(rsa);
    // destinction buffer is too small
    if (dst_size < len) {
        ret = 0;
        goto free;
    }
        
    ret = RSA_public_encrypt(src_size, src, dst, rsa, RSA_PKCS1_PADDING);
    if (ret < 0) {
        goto free;
    }

    ret = len;

free:
    BIO_free_all(pub);
    RSA_free(rsa);
    return ret;
}

uint64_t spk_rsa_pri_decrypt(const uint8_t* src, const uint64_t src_size, uint8_t* dst,
    uint64_t dst_size, const int8_t* pri_key) {
    int ret = 0;
    BIO* pri = BIO_new_mem_buf(pri_key, -1);
    RSA* rsa = NULL;
    rsa = PEM_read_bio_RSAPrivateKey(pri, &rsa, NULL, NULL);

    int len = RSA_size(rsa);
    // destinction buffer is too small
    if (dst_size < len) {
        ret = 0;
        goto free;
    }

    ret = RSA_private_decrypt(src_size, src, dst, rsa, RSA_PKCS1_PADDING);
    if (ret < 0) {
        goto free;
    }

    ret = len;

free:
    BIO_free_all(pri);
    RSA_free(rsa);
    return ret;
}

/* return sign_size */
uint64_t spk_rsa_pri_sign(const uint8_t* src, const uint64_t src_size, int8_t* sign,
    uint64_t sign_alloc_size, const int8_t* pri_key) {
    uint64_t ret = 0;
    uint8_t tmp_hash[64];
   
    BIO* pri = BIO_new_mem_buf(pri_key, -1);
    RSA* rsa = NULL;
    rsa = PEM_read_bio_RSAPrivateKey(pri, &rsa, NULL, NULL);

    // check destinction buffer enough or not
    /* Gosh! You must set sign_size to 0, Its use LEFT-SHIFT to do init. */
    uint32_t sign_size = 0;
    uint64_t __rsa_size = RSA_size(rsa);
    uint64_t __base64_size = spk_base64_enbound(__rsa_size);
    if (sign_alloc_size < __base64_size) {
        ret = 0;
        goto free;
    }

    uint8_t* sign_bin = malloc(__base64_size);
    if (!sign_bin) {
        ret = 0;
        goto free;
    }
    
    SHA1(src, src_size, tmp_hash);
    ret = RSA_sign(NID_sha1, tmp_hash, 20, sign_bin, &sign_size, rsa);
    if (ret != 1) {
        free(sign_bin);
        goto free;
    }
    ret = spk_base64_encode(sign_bin, sign_size, sign);

free:
    BIO_free_all(pri);
    RSA_free(rsa);
    return ret;
}

/* return 1 while verify success, otherwise failed */
int spk_rsa_pub_verify(const uint8_t* src, const uint64_t src_size, const int8_t* sign,
    const uint64_t sign_size, const int8_t* pub_key) {
    int ret = 0;
    uint8_t tmp_hash[64];

    BIO* pub = BIO_new_mem_buf(pub_key, -1);
    RSA* rsa = NULL;
    rsa = PEM_read_bio_RSAPublicKey(pub, &rsa, NULL, NULL);
    SHA1(src, src_size, tmp_hash);

    uint8_t* debase = malloc(spk_base64_debound(sign_size));
    if (!debase) {
        ret = -1;
        goto free;
    }
    int debase_size = spk_base64_decode(sign, sign_size, debase);
    ret = RSA_verify(NID_sha1, tmp_hash, 20, debase, debase_size, rsa);

    free(debase);

free:
    BIO_free_all(pub);
    RSA_free(rsa);
    return ret;
}
