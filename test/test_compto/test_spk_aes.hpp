#ifndef SPARKLE_TEST_SPK_AES_HPP_
#define SPARKLE_TEST_SPK_AES_HPP_

#include <gtest/gtest.h>
#include <spk_compto.h>

TEST(test_spk_aes, case_aes_bound_1) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    const char* key = "keys";

    for (int i = 0; i < 65537; i++) {
        ASSERT_EQ((i / 16 * 16 + (i % 16 ? 16 : 0)), spk_aes_bound(i));
    }
    ASSERT_EQ(1024, spk_aes_bound(1021));
    ASSERT_EQ(1040, spk_aes_bound(1025));

    for (int i = 1; i <= 1024; i++) {
        int ret = spk_aes_cbc256_encrypt(buf1, i, buf2, 1024, key);
        ASSERT_EQ(spk_aes_bound(i), ret);
    }
}

TEST(test_spk_aes, case_aes_cbc256_encrypt_case_1) {
    uint8_t buf1[1024];
    uint8_t buf2[1024];
    uint8_t buf3[1024];
    const char* key = "keyskeyskeyskeyskeyskeyskeyskeyskeyskeyskeyskeyskeyskeyskeyskeys";
    for (int i = 1; i <= 1024; i++) {
        int ret1 = spk_aes_cbc256_encrypt(buf1, i, buf2, 1024, key);
        int ret2 = spk_aes_cbc256_decrypt(buf2, ret1, buf3, 1024, key);
        for (int j = 0; j < i; j++) {
            EXPECT_EQ(buf1[j], buf3[j]);
        }
        memcpy(buf1, buf2, i);
    }
}

#endif // SPARKLE_TEST_SPK_AES_HPP_