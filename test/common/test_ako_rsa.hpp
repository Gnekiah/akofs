#ifndef AKOFS_TEST_AKO_RSA_HPP_
#define AKOFS_TEST_AKO_RSA_HPP_

#include <gtest/gtest.h>
#include <ako_compto.h>

TEST(test_ako_rsa, case_rsa_generate_key_pairs_1) {
    int8_t pri_key[2048];
    uint64_t pri_key_size = 2048;
    int8_t pub_key[2048];
    uint64_t pub_key_size = 2048;

    int ret = ako_rsa_generate_keypair_2048(pri_key, &pri_key_size, pub_key, &pub_key_size);
    ASSERT_EQ(0, ret);
    EXPECT_LT(0, pri_key_size);
    EXPECT_LT(0, pub_key_size);
}

TEST(test_ako_rsa, case_rsa_pub_encrypt_and_pri_decrypt_1) {
    int8_t pri_key[2048];
    uint64_t pri_key_size = 2048;
    int8_t pub_key[2048];
    uint64_t pub_key_size = 2048;

    int ret1 = ako_rsa_generate_keypair_2048(pri_key, &pri_key_size, pub_key, &pub_key_size);
    ASSERT_EQ(0, ret1);

    const char* plain1 = "an oops is a deviation from correct behavior of the Linux kernel, \
one that produces a certain error log.";
    int plain1_size = strlen(plain1);
    char cipher1[4096];
    char plain2[4096];
    int ret2 = ako_rsa_pub_encrypt((const uint8_t*)plain1, plain1_size, (uint8_t*)cipher1, 4096, pub_key);
    EXPECT_LT(0, ret2);
    int ret3 = ako_rsa_pri_decrypt((const uint8_t*)cipher1, ret2, (uint8_t*)plain2, 4096, pri_key);
    EXPECT_LT(0, ret3);
    plain2[plain1_size] = 0;
    EXPECT_STREQ(plain1, plain2);
}

TEST(test_ako_rsa, case_rsa_pub_encrypt_and_pri_decrypt_2) {
    int8_t pri_key[2048];
    uint64_t pri_key_size = 2048;
    int8_t pub_key[1024];
    uint64_t pub_key_size = 1024;

    int ret1 = ako_rsa_generate_keypair_2048(pri_key, &pri_key_size, pub_key, &pub_key_size);
    ASSERT_EQ(0, ret1);

    /* mmp, while the src_size is bigger than 245 bytes, rsa pub encrypt would not work. */
    const int PLANE_SIZE = 245;
    uint8_t plain1[PLANE_SIZE];
    int plain1_size = PLANE_SIZE;
    for (int i = 0; i < PLANE_SIZE; i++) {
        plain1[i] = (uint8_t)rand();
    }

    uint8_t cipher1[2048];
    uint8_t plain2[2048];
    int ret2 = ako_rsa_pub_encrypt(plain1, plain1_size, cipher1, 2048, pub_key);
    EXPECT_LT(0, ret2);
    int ret3 = ako_rsa_pri_decrypt(cipher1, ret2, plain2, 2048, pri_key);
    EXPECT_LT(0, ret3);
    
    for (int i = 0; i < plain1_size; i++) {
        EXPECT_EQ(plain1[i], plain2[i]);
    }
}

TEST(test_ako_rsa, case_rsa_pri_sign_and_pub_check_1) {
    int8_t pri_key[2048];
    uint64_t pri_key_size = 2048;
    int8_t pub_key[2048];
    uint64_t pub_key_size = 2048;

    int ret1 = ako_rsa_generate_keypair_2048(pri_key, &pri_key_size, pub_key, &pub_key_size);
    ASSERT_EQ(0, ret1);

    const char* plain1 = "an oops is a deviation from correct behavior of the Linux kernel, \
one that produces a certain error log.";
    int plain1_size = strlen(plain1);
    int8_t sign[1024];
    int ret2 = ako_rsa_pri_sign((const uint8_t*)plain1, plain1_size, sign, 1024, pri_key);
    EXPECT_LT(0, ret2);
    int ret3 = ako_rsa_pub_verify((const uint8_t*)plain1, plain1_size, sign, ret2, pub_key);
    EXPECT_EQ(1, ret3);
}

TEST(test_ako_rsa, case_rsa_pri_sign_and_pub_check_2) {
    int8_t pri_key[2048];
    uint64_t pri_key_size = 2048;
    int8_t pub_key[1024];
    uint64_t pub_key_size = 1024;

    int ret1 = ako_rsa_generate_keypair_2048(pri_key, &pri_key_size, pub_key, &pub_key_size);
    ASSERT_EQ(0, ret1);

    const int PLANE_SIZE = 4096;
    uint8_t plain1[PLANE_SIZE];
    int plain1_size = PLANE_SIZE;
    for (int i = 0; i < PLANE_SIZE; i++) {
        plain1[i] = (uint8_t)rand();
    }

    int8_t sign[1024];
    int ret2 = ako_rsa_pri_sign(plain1, plain1_size, sign, 1024, pri_key);
    EXPECT_LT(0, ret2);
    int ret3 = ako_rsa_pub_verify(plain1, plain1_size, sign, ret2, pub_key);
    EXPECT_EQ(1, ret3);
}

#endif // AKOFS_TEST_AKO_RSA_HPP_
