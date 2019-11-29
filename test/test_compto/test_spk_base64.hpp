#ifndef SPARKLE_TEST_SPK_BASE64_HPP_
#define SPARKLE_TEST_SPK_BASE64_HPP_

#include <gtest/gtest.h>
#include <spk_compto.h>
#include <string>

TEST(test_spk_base64, case_string_1) {
    char buf[64] = { 0 };
    uint64_t buf1_size = spk_base64_encode((const uint8_t*)"", 0, (int8_t*)buf);
    EXPECT_STREQ("", buf);
    ASSERT_EQ(spk_base64_enbound(0), buf1_size);
    buf1_size = spk_base64_encode((const uint8_t*)"f", 1, (int8_t*)buf);
    EXPECT_STREQ("Zg==", buf);
    ASSERT_EQ(spk_base64_enbound(1), buf1_size);
    buf1_size = spk_base64_encode((const uint8_t*)"fo", 2, (int8_t*)buf);
    EXPECT_STREQ("Zm8=", buf);
    ASSERT_EQ(spk_base64_enbound(2), buf1_size);
    buf1_size = spk_base64_encode((const uint8_t*)"foo", 3, (int8_t*)buf);
    EXPECT_STREQ("Zm9v", buf);
    ASSERT_EQ(spk_base64_enbound(3), buf1_size);
    buf1_size = spk_base64_encode((const uint8_t*)"foob", 4, (int8_t*)buf);
    EXPECT_STREQ("Zm9vYg==", buf);
    ASSERT_EQ(spk_base64_enbound(4), buf1_size);
    buf1_size = spk_base64_encode((const uint8_t*)"fooba", 5, (int8_t*)buf);
    EXPECT_STREQ("Zm9vYmE=", buf);
    ASSERT_EQ(spk_base64_enbound(5), buf1_size);
    buf1_size = spk_base64_encode((const uint8_t*)"foobar", 6, (int8_t*)buf);
    EXPECT_STREQ("Zm9vYmFy", buf);
    ASSERT_EQ(spk_base64_enbound(6), buf1_size);
}

TEST(test_spk_base64, case_string_2) {
    char buf[64] = { 0 };
    uint64_t buf1_size = spk_base64_decode((const int8_t*)"", 0, (uint8_t*)buf);
    ASSERT_EQ(0, buf1_size);
    EXPECT_STREQ("", buf);
    ASSERT_LE(spk_base64_debound(0) - buf1_size, 2);
    buf1_size = spk_base64_decode((const int8_t*)"Zg==", 4, (uint8_t*)buf);
    ASSERT_EQ(1, buf1_size);
    EXPECT_STREQ("f", buf);
    ASSERT_LE(spk_base64_debound(4) - buf1_size, 2);
    buf1_size = spk_base64_decode((const int8_t*)"Zm8=", 4, (uint8_t*)buf);
    ASSERT_EQ(2, buf1_size);
    EXPECT_STREQ("fo", buf);
    ASSERT_LE(spk_base64_debound(4) - buf1_size, 2);
    buf1_size = spk_base64_decode((const int8_t*)"Zm9v", 4, (uint8_t*)buf);
    ASSERT_EQ(3, buf1_size);
    EXPECT_STREQ("foo", buf);
    ASSERT_LE(spk_base64_debound(4) - buf1_size, 2);
    buf1_size = spk_base64_decode((const int8_t*)"Zm9vYg==", 8, (uint8_t*)buf);
    ASSERT_EQ(4, buf1_size);
    EXPECT_STREQ("foob", buf);
    ASSERT_LE(spk_base64_debound(8) - buf1_size, 2);
    buf1_size = spk_base64_decode((const int8_t*)"Zm9vYmE=", 8, (uint8_t*)buf);
    ASSERT_EQ(5, buf1_size);
    EXPECT_STREQ("fooba", buf);
    ASSERT_LE(spk_base64_debound(8) - buf1_size, 2);
    buf1_size = spk_base64_decode((const int8_t*)"Zm9vYmFy", 8, (uint8_t*)buf);
    ASSERT_EQ(6, buf1_size);
    EXPECT_STREQ("foobar", buf);
    ASSERT_LE(spk_base64_debound(8) - buf1_size, 2);
}

TEST(test_spk_base64, case_bytes_buffer_1) {
    uint8_t buf1[1024];
    int8_t buf2[2048];
    uint8_t buf3[2048];
    for (int i = 0; i < 1024; i++) {
        buf1[i] = (uint8_t)rand();
    }
    uint64_t buf1_size = spk_base64_encode((const uint8_t*)buf1, 1024, (int8_t*)buf2);
    ASSERT_EQ(spk_base64_enbound(1024), buf1_size);
    uint64_t buf2_size = spk_base64_decode((const int8_t*)buf2, buf1_size, (uint8_t*)buf3);
    ASSERT_LE(spk_base64_debound(buf1_size) - buf2_size, 2);
    ASSERT_EQ(1024, buf2_size);
    for (int i = 0; i < 1024; i++) {
        ASSERT_EQ(buf1[i], buf3[i]);
    }
}

#endif // SPARKLE_TEST_SPK_BASE64_HPP_