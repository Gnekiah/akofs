#ifndef AKOFS_TEST_AKO_DES_HPP_
#define AKOFS_TEST_AKO_DES_HPP_

#include <gtest/gtest.h>
#include <ako_compto.h>

TEST(test_ako_des, case_des_bound_1) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    const char* key = "keys";

    for (int i = 0; i < 65537; i++) {
        ASSERT_EQ((i / 8 * 8 + (i % 8 ? 8 : 0)), ako_des_bound(i));
    }
    ASSERT_EQ(1024, ako_des_bound(1021));
    ASSERT_EQ(1032, ako_des_bound(1025));

    for (int i = 0; i <= 1024; i++) {
        int ret = ako_des_ecb_encrypt(buf1, i, buf2, key);
        ASSERT_EQ(ako_des_bound(i), ret);
    }
}

TEST(test_ako_des, case_des_ecb_encrypt_1) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    const char *key = "keys";
    for (int i = 0; i < 1024; i++) {
        buf1[i] = (uint8_t)rand();
    }
    int ret1 = ako_des_ecb_encrypt(buf1, 1024, buf2, key);
    int ret2 = ako_des_ecb_decrypt(buf2, 1024, buf3, key);

    ASSERT_EQ(ret1, ret2);
    for (int i = 0; i < 1024; i++) {
        ASSERT_EQ(buf1[i], buf3[i]);
    }
}

TEST(test_ako_des, case_des_ecb_encrypt_2) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    char key[32];
    strcpy(key, "keyskeyskeys");

    for (int i = 12; i > 0; i--) {
        key[i] = 0;
        for (int j = 1; j < 1024; j+=7) {
            int ret1 = ako_des_ecb_encrypt(buf1, j, buf2, key);
            ASSERT_EQ(ret1, ako_des_bound(j));
            int ret2 = ako_des_ecb_decrypt(buf2, ret1, buf3, key);
            ASSERT_EQ(ret1, ret2);
            for (int k = 0; k < j; k++) {
                ASSERT_EQ(buf1[k], buf3[k]);
            }
        }
    }
}

TEST(test_ako_des, case_des_ncbc_encrypt_1) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    const char* key = "keys";
    for (int i = 0; i < 1024; i++) {
        buf1[i] = (uint8_t)rand();
    }
    int ret1 = ako_des_ncbc_encrypt(buf1, 1024, buf2, key);
    int ret2 = ako_des_ncbc_decrypt(buf2, 1024, buf3, key);

    ASSERT_EQ(ret1, ret2);
    for (int i = 0; i < 1024; i++) {
        ASSERT_EQ(buf1[i], buf3[i]);
    }
}

TEST(test_ako_des, case_des_ncbc_encrypt_2) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    char key[32];
    strcpy(key, "keyskeyskeys");

    for (int i = 12; i > 0; i--) {
        key[i] = 0;
        for (int j = 1; j < 1024; j += 7) {
            int ret1 = ako_des_ncbc_encrypt(buf1, j, buf2, key);
            ASSERT_EQ(ret1, ako_des_bound(j));
            int ret2 = ako_des_ncbc_decrypt(buf2, ret1, buf3, key);
            ASSERT_EQ(ret1, ret2);
            for (int k = 0; k < j; k++) {
                ASSERT_EQ(buf1[k], buf3[k]);
            }
        }
    }
}

#endif // AKOFS_TEST_AKO_DES_HPP_
