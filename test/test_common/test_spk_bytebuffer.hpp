#ifndef SPARKLE_TEST_SPK_BYTE_BUFFER_HPP_
#define SPARKLE_TEST_SPK_BYTE_BUFFER_HPP_

#include <gtest/gtest.h>
#include <spk_bytebuffer.h>

TEST(test_spk_bytebuffer, case_bytebuffer_new_test_1) {
    int ret = 0;
    const char* const_chars = "Forget about the days when it's been cloudy. But \
don't forget your hours in the sun.";
    const char* const_hexs = "466f726765742061626f7574207468652064617973207768656\
e2069742773206265656e20636c6f7564792e2042757420646f6e277420666f7267657420796f7572\
20686f75727320696e207468652073756e2e";
    char print_buff[1024];
    memset(print_buff, 0, 1024);

    spk_byte_buffer* buff1 = spk_bytebuf_new_copy((uint8_t*)const_chars, strlen(const_chars));
    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ(const_chars, print_buff);

    spk_bytebuf_print_hex(buff1, print_buff);
    EXPECT_STREQ(const_hexs, print_buff);

    spk_byte_buffer* buff2 = spk_bytebuf_new_wrap((uint8_t*)print_buff, strlen(print_buff));
    char* tmp_buff = (char*)spk_bytebuf_get_bytes(buff2, 10);
    char tmp_char[11];
    tmp_char[10] = 0;
    memcpy(tmp_char, tmp_buff, 10);
    EXPECT_NE(0, (uint64_t)tmp_buff);
    EXPECT_STREQ("466f726765", tmp_char);
    free(tmp_buff);

    spk_byte_buffer* buff3 = spk_bytebuf_new(2048);
    spk_bytebuf_put_bytebuf(buff3, buff1);
    spk_byte_buffer* buff5 = spk_bytebuf_resize(buff3, 10);
    EXPECT_EQ(0, (uint64_t)buff5);

    buff5 = spk_bytebuf_resize_force(buff3, 12);
    EXPECT_NE(0, (uint64_t)buff5);

    spk_bytebuf_print_ascii(buff3, print_buff);
    EXPECT_STREQ("Forget about", print_buff);
    EXPECT_TRUE(spk_bytebuf_is_full(buff3));
    EXPECT_FALSE(spk_bytebuf_is_empty(buff3));

    buff5 = spk_bytebuf_resize(buff3, 18);
    EXPECT_NE(0, (uint64_t)buff5);
    EXPECT_EQ(18, spk_bytebuf_size(buff3));
    EXPECT_EQ(12, spk_bytebuf_length(buff3));
    EXPECT_EQ(6, spk_bytebuf_valid(buff3));

    spk_byte_buffer* buff4 = spk_bytebuf_new_default();
    EXPECT_TRUE(spk_bytebuf_is_empty(buff4));
    EXPECT_FALSE(spk_bytebuf_is_full(buff4));

    spk_bytebuf_free(buff4);
    spk_bytebuf_free(buff3);
    spk_bytebuf_free(buff2);
    spk_bytebuf_free(buff1);
}

TEST(test_spk_bytebuffer, case_bytebuffer_boundary_condition_test_1) {
    int ret = 0;
    const char* const_chars = "Forget about the days when it's been cloudy. But \
don't forget your hours in the sun.";;
    char print_buff[1024];
    memset(print_buff, 0, 1024);

    spk_byte_buffer* buff1 = spk_bytebuf_new_default();
    spk_byte_buffer* buff2 = spk_bytebuf_resize_force(buff1, 64);
    EXPECT_TRUE(buff2);

    spk_bytebuf_skip(buff1, 10086);
    EXPECT_EQ(0, spk_bytebuf_pos(buff1));

    spk_bytebuf_put_bytes(buff1, (uint8_t*)const_chars, strlen(const_chars));
    spk_bytebuf_skip(buff1, 10086);
    EXPECT_EQ(64, spk_bytebuf_pos(buff1));

    buff2 = spk_bytebuf_clone(buff1);
    EXPECT_TRUE(buff2);
    EXPECT_TRUE(spk_bytebuf_equals(buff1, buff2));

    ret = spk_bytebuf_clear(buff2);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, spk_bytebuf_pos(buff2));
    EXPECT_EQ(0, spk_bytebuf_length(buff2));
    EXPECT_EQ(64, spk_bytebuf_size(buff2));

    spk_byte_buffer* buff3 = spk_bytebuf_resize_force(buff1, 12);
    spk_bytebuf_put_bytebuf(buff2, buff1);
    EXPECT_TRUE(buff3);
    buff3 = spk_bytebuf_resize_force(buff2, 12);
    EXPECT_TRUE(buff3);
    EXPECT_TRUE(spk_bytebuf_equals(buff1, buff2));

    uint8_t ch = spk_bytebuf_peek(buff1);
    EXPECT_EQ('F', ch);
    ch = spk_bytebuf_get(buff1);
    EXPECT_EQ('F', ch);
    ch = spk_bytebuf_peek(buff1);
    EXPECT_EQ('o', ch);
    ch = spk_bytebuf_get(buff1);
    EXPECT_EQ('o', ch);
    ch = spk_bytebuf_peek(buff1);
    EXPECT_EQ('r', ch);
    ch = spk_bytebuf_get(buff1);
    EXPECT_EQ('r', ch);
    ch = spk_bytebuf_get_at(buff1, 10);
    EXPECT_EQ('u', ch);
    ch = spk_bytebuf_get_at(buff1, 11);
    EXPECT_EQ('t', ch);

    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Forget about", print_buff);

    spk_bytebuf_replace(buff1, 'o', 't', 1, false);
    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Ftrget abtut", print_buff);

    spk_bytebuf_replace(buff1, 't', 'G', 3, false);
    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("FtrgeG abGuG", print_buff);

    spk_bytebuf_replace(buff1, 'G', '=', 3, true);
    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Ftrge= abGuG", print_buff);

    spk_bytebuf_free(buff2);
    spk_bytebuf_free(buff1);
}

TEST(test_spk_bytebuffer, case_bytebuffer_boundary_condition_test_2) {
    int ret = 0;
    const char* const_chars = "Forget about the days when it's been cloudy. But \
don't forget your hours in the sun.";
    char print_buff[1024];
    memset(print_buff, 0, 1024);

    spk_byte_buffer* buff1 = spk_bytebuf_new_default();
    spk_bytebuf_put_bytes(buff1, (uint8_t*)const_chars, strlen(const_chars));
    spk_byte_buffer* buff2 = spk_bytebuf_resize_force(buff1, 32);
    EXPECT_TRUE(buff2);

    spk_bytebuf_get_bytes_in(buff1, (uint8_t*)print_buff, 26);
    EXPECT_STREQ("Forget about the days when", print_buff);

    memset(print_buff, 0, 1024);
    spk_bytebuf_get_bytes_at_in(buff1, 7, (uint8_t*)print_buff, 20);
    EXPECT_STREQ("about the days when ", print_buff);

    uint8_t* tmp_buf = spk_bytebuf_get_bytes_at(buff1, 20, 7);
    EXPECT_TRUE(tmp_buf);
    memset(print_buff, 0, 1024);
    memcpy(print_buff, tmp_buf, 20);
    EXPECT_STREQ("about the days when ", print_buff);

    spk_bytebuf_resize(buff1, 40);
    spk_bytebuf_put(buff1, 'X');
    memset(print_buff, 0, 1024);
    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Forget about the days when it's X", print_buff);
    
    spk_bytebuf_put_at(buff1, 'Z', 0);
    memset(print_buff, 0, 1024);
    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Zorget about the days when it's X", print_buff);

    spk_bytebuf_put_bytes(buff1, (uint8_t*)"FK", 2);
    memset(print_buff, 0, 1024);
    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Zorget about the days when it's XFK", print_buff);

    spk_bytebuf_put_bytes_at(buff1, (uint8_t*)"CUCKOO", 6, 7);
    memset(print_buff, 0, 1024);
    spk_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Zorget CUCKOOthe days when it's XFK", print_buff);

    spk_bytebuf_free(buff1);
}

TEST(test_spk_bytebuffer, case_bytebuffer_boundary_condition_test_3) {
    int ret = 0;
    char print_buff[1024];
    memset(print_buff, 0, 1024);

    spk_byte_buffer* buff1 = spk_bytebuf_new_default();
    spk_byte_buffer* buff2 = spk_bytebuf_resize_force(buff1, 40);
    EXPECT_TRUE(buff2);
    EXPECT_EQ(0, spk_bytebuf_pos(buff1));
    EXPECT_EQ(0, spk_bytebuf_length(buff1));
    EXPECT_EQ(40, spk_bytebuf_size(buff1));

    spk_bytebuf_put_double(buff1, 101005.01);
    EXPECT_EQ(sizeof(double), spk_bytebuf_length(buff1));
    spk_bytebuf_put_float(buff1, (float)102.02);
    EXPECT_EQ(sizeof(double) + sizeof(float), spk_bytebuf_length(buff1));
    spk_bytebuf_put_int(buff1, 103);
    EXPECT_EQ(sizeof(double) + sizeof(float) + sizeof(uint32_t), spk_bytebuf_length(buff1));
    spk_bytebuf_put_long(buff1, 10086);
    EXPECT_EQ(sizeof(double) + sizeof(float) + sizeof(uint32_t) + sizeof(uint64_t),
        spk_bytebuf_length(buff1));
    spk_bytebuf_put_short(buff1, 265);
    EXPECT_EQ(sizeof(double) + sizeof(float) + sizeof(uint32_t) + sizeof(uint64_t) 
        + sizeof(uint16_t), spk_bytebuf_length(buff1));

    spk_bytebuf_reset(buff1);
    EXPECT_NEAR(101005.01, spk_bytebuf_get_double(buff1), 0.1);
    EXPECT_NEAR(102.02, spk_bytebuf_get_float(buff1), 0.1);
    EXPECT_EQ(103, spk_bytebuf_get_int(buff1));
    EXPECT_EQ(10086, spk_bytebuf_get_long(buff1));
    EXPECT_EQ(265, spk_bytebuf_get_short(buff1));

    spk_bytebuf_put_double_at(buff1, 23628.2345, 1);
    spk_bytebuf_put_float_at(buff1, (float)348.324, 9);
    spk_bytebuf_put_int_at(buff1, 6745, 18);
    spk_bytebuf_put_long_at(buff1, 564313, 27);
    spk_bytebuf_put_short_at(buff1, 23456, 36);

    EXPECT_NEAR(23628.2345, spk_bytebuf_get_double_at(buff1, 1), 0.1);
    EXPECT_NEAR(348.324, spk_bytebuf_get_float_at(buff1, 9), 0.1);
    EXPECT_EQ(6745, spk_bytebuf_get_int_at(buff1, 18));
    EXPECT_EQ(564313, spk_bytebuf_get_long_at(buff1, 27));
    EXPECT_EQ(23456, spk_bytebuf_get_short_at(buff1, 36));

    spk_bytebuf_free(buff1);
}

#endif // SPARKLE_TEST_SPK_BYTE_BUFFER_HPP_
