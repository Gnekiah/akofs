#ifndef SPARKLE_TEST_SPK_BUILT_IN_HPP_
#define SPARKLE_TEST_SPK_BUILT_IN_HPP_

#include<gtest/gtest.h>
#include<spk_builtin.h>

#ifdef _MSC_VER
#include <intrin.h>
static unsigned long long __bit_scan_forward(unsigned long long s) {
    unsigned long r = 0;
    _BitScanForward64(&r, s);
    return r;
}
#endif

TEST(test_spk_builtin, case_bytebuffer_new_test_1) {
    int64_t wlb;
    wlb = 0b0000000011101010101101;

#ifdef _MSC_VER
    EXPECT_EQ(9, __popcnt64(wlb));
    EXPECT_EQ(0, __bit_scan_forward(wlb));
    wlb = 0b00000000111010101011010000100;
    EXPECT_EQ(10, __popcnt64(wlb));
    EXPECT_EQ(2, __bit_scan_forward(wlb));
    wlb = 0b000000001110101010110100001000000000011;
    EXPECT_EQ(12, __popcnt64(wlb));
    EXPECT_EQ(0, __bit_scan_forward(wlb));
    wlb = 0b0000000011101010101101000010011111111;
    EXPECT_EQ(18, __popcnt64(wlb));
    EXPECT_EQ(0, __bit_scan_forward(wlb));
    wlb = 0b1000000000000000000000000000000000000000000000000000000;
    EXPECT_EQ(1, __popcnt64(wlb));
    EXPECT_EQ(54, __bit_scan_forward(wlb));
    wlb = 0b000000000000000000000000000000000000000000000000000000;
    EXPECT_EQ(0, __popcnt64(wlb));
    EXPECT_EQ(0, __bit_scan_forward(wlb));
#endif

    wlb = 0b0000000011101010101101;
    EXPECT_EQ(9, spk_builtin_popcount64(wlb));
    EXPECT_EQ(0, spk_builtin_ctzll(wlb));
    wlb = 0b00000000111010101011010000100;
    EXPECT_EQ(10, spk_builtin_popcount64(wlb));
    EXPECT_EQ(2, spk_builtin_ctzll(wlb));
    wlb = 0b000000001110101010110100001000000000011;
    EXPECT_EQ(12, spk_builtin_popcount64(wlb));
    EXPECT_EQ(0, spk_builtin_ctzll(wlb));
    wlb = 0b0000000011101010101101000010011111111;
    EXPECT_EQ(18, spk_builtin_popcount64(wlb));
    EXPECT_EQ(0, spk_builtin_ctzll(wlb));
    wlb = 0b1000000000000000000000000000000000000000000000000000000;
    EXPECT_EQ(1, spk_builtin_popcount64(wlb));
    EXPECT_EQ(54, spk_builtin_ctzll(wlb));
    wlb = 0b000000000000000000000000000000000000000000000000000000;
    EXPECT_EQ(0, spk_builtin_popcount64(wlb));
    EXPECT_EQ(0, spk_builtin_ctzll(wlb));

}

#endif // SPARKLE_TEST_SPK_BUILT_IN_HPP_