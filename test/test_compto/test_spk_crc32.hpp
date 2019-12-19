#ifndef SPARKLE_TEST_SPK_CRC32_HPP_
#define SPARKLE_TEST_SPK_CRC32_HPP_

#include <gtest/gtest.h>
#include <spk_compto.h>

TEST(test_spk_crc32, case_crc32_1) {
    uint32_t crc = spk_crc32(0, NULL, 0);
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    crc = spk_crc32(crc, (uint8_t*)buf, size);
    ASSERT_EQ((uint64_t)2964150809, (uint64_t)crc);
}

TEST(test_spk_crc32, case_crc32_2) {
    uint32_t crc = spk_crc32(0, NULL, 0);
    const char* buf = "buffer test for the next";
    uint32_t size = 24;
    crc = spk_crc32(crc, (uint8_t*)buf, size);
    ASSERT_EQ((uint64_t)2497872571, (uint64_t)crc);
}

TEST(test_spk_crc32, case_crc32_combine_1) {
    uint32_t crc1 = spk_crc32(0, NULL, 0);
    uint32_t crc2 = spk_crc32(0, NULL, 0);
    const char* buf1 = "this is a buffer";
    const char* buf2 = "buffer test for the next";
    uint32_t size1 = 16;
    uint32_t size2 = 24;
    crc1 = spk_crc32(crc1, (uint8_t*)buf1, size1);
    crc2 = spk_crc32(crc2, (uint8_t*)buf2, size2);
    uint32_t crc_ret1 = spk_crc32_combine(crc1, crc2, size2);
    ASSERT_EQ((uint64_t)2316370935, (uint64_t)crc_ret1);
}

TEST(test_spk_crc32, case_crc32_combine_2) {
    const char* buf1 = "this is a buffer";
    uint32_t size1 = 16;
    uint32_t crc1 = spk_crc32(0, NULL, 0);
    crc1 = spk_crc32(crc1, (uint8_t*)buf1, size1);

    const char* buf2 = "buffer test for the next";
    uint32_t size2 = 24;
    uint32_t crc2 = spk_crc32(0, NULL, 0);
    crc2 = spk_crc32(crc2, (uint8_t*)buf2, size2);
    
    uint32_t crc_ret1 = spk_crc32_combine(crc2, crc1, size1);

    const char* buf3 = "buffer test for the nextthis is a buffer";
    uint32_t size3 = 40;
    uint32_t crc3 = spk_crc32(0, NULL, 0);
    crc3 = spk_crc32(crc3, (uint8_t*)buf3, size3);
    ASSERT_EQ((uint64_t)crc3, (uint64_t)crc_ret1);
}

TEST(test_spk_cec32, case_crc32_combine_for_series_case_1) {
    uint64_t id = 0xf134956824505621;
    uint64_t dst_id[8];

    uint32_t crc0 = spk_crc32(0, NULL, 0);
    for (int i = 0; i < 8; i++) {
        uint64_t crc1 = crc0 + id;
        crc0 = spk_crc32(crc0, (const uint8_t*)& crc1, 8);
        dst_id[i] = crc0;
        crc1 = crc0 + id;
        crc0 = spk_crc32(crc0, (const uint8_t*)& crc1, 8);
        dst_id[i] = dst_id[i] << 32 | crc0;
    }
    EXPECT_NE(id, dst_id[0]);
    EXPECT_NE(dst_id[0], dst_id[1]);
    EXPECT_NE(dst_id[1], dst_id[2]);
    EXPECT_NE(dst_id[2], dst_id[3]);
    EXPECT_NE(dst_id[3], dst_id[4]);
    EXPECT_NE(dst_id[4], dst_id[5]);
    EXPECT_NE(dst_id[5], dst_id[6]);
    EXPECT_NE(dst_id[6], dst_id[7]);
}

#endif // SPARKLE_TEST_SPK_CRC32_HPP_
