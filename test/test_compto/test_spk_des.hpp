#ifndef SPARKLE_TEST_SPK_DES_HPP_
#define SPARKLE_TEST_SPK_DES_HPP_

#include <gtest/gtest.h>
#include <spk_compto.h>

TEST(test_spk_des, case_des_bound_1) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    const char* key = "keys";

    for (int i = 0; i < 65537; i++) {
        ASSERT_EQ((i / 8 * 8 + (i % 8 ? 8 : 0)), spk_des_bound(i));
    }
    ASSERT_EQ(1024, spk_des_bound(1021));
    ASSERT_EQ(1032, spk_des_bound(1025));

    for (int i = 0; i <= 1024; i++) {
        int ret = spk_des_ecb_encrypt(buf1, i, buf2, key);
        ASSERT_EQ(spk_des_bound(i), ret);
    }
}

TEST(test_spk_des, case_des_ecb_encrypt_1) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    const char *key = "keys";
    for (int i = 0; i < 1024; i++) {
        buf1[i] = (uint8_t)rand();
    }
    int ret1 = spk_des_ecb_encrypt(buf1, 1024, buf2, key);
    int ret2 = spk_des_ecb_decrypt(buf2, 1024, buf3, key);

    ASSERT_EQ(ret1, ret2);
    for (int i = 0; i < 1024; i++) {
        ASSERT_EQ(buf1[i], buf3[i]);
    }
}

TEST(test_spk_des, case_des_ecb_encrypt_2) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    char key[32];
    strcpy(key, "keyskeyskeys");

    for (int i = 12; i > 0; i--) {
        key[i] = 0;
        for (int j = 1; j < 1024; j+=7) {
            int ret1 = spk_des_ecb_encrypt(buf1, j, buf2, key);
            ASSERT_EQ(ret1, spk_des_bound(j));
            int ret2 = spk_des_ecb_decrypt(buf2, ret1, buf3, key);
            ASSERT_EQ(ret1, ret2);
            for (int k = 0; k < j; k++) {
                ASSERT_EQ(buf1[k], buf3[k]);
            }
        }
    }
}

TEST(test_spk_des, case_des_ncbc_encrypt_1) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    const char* key = "keys";
    for (int i = 0; i < 1024; i++) {
        buf1[i] = (uint8_t)rand();
    }
    int ret1 = spk_des_ncbc_encrypt(buf1, 1024, buf2, key);
    int ret2 = spk_des_ncbc_decrypt(buf2, 1024, buf3, key);

    ASSERT_EQ(ret1, ret2);
    for (int i = 0; i < 1024; i++) {
        ASSERT_EQ(buf1[i], buf3[i]);
    }
}

TEST(test_spk_des, case_des_ncbc_encrypt_2) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    char key[32];
    strcpy(key, "keyskeyskeys");

    for (int i = 12; i > 0; i--) {
        key[i] = 0;
        for (int j = 1; j < 1024; j += 7) {
            int ret1 = spk_des_ncbc_encrypt(buf1, j, buf2, key);
            ASSERT_EQ(ret1, spk_des_bound(j));
            int ret2 = spk_des_ncbc_decrypt(buf2, ret1, buf3, key);
            ASSERT_EQ(ret1, ret2);
            for (int k = 0; k < j; k++) {
                ASSERT_EQ(buf1[k], buf3[k]);
            }
        }
    }
}

#endif // SPARKLE_TEST_SPK_DES_HPP_
