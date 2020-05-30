#ifndef AKOFS_TEST_AKO_BYTE_BUFFER_HPP_
#define AKOFS_TEST_AKO_BYTE_BUFFER_HPP_

#include <gtest/gtest.h>
#include <ako_bytebuffer.h>

TEST(test_ako_bytebuffer, case_bytebuffer_new_test_1) {
    int ret = 0;
    const char* const_chars = "Forget about the days when it's been cloudy. But \
don't forget your hours in the sun.";
    const char* const_hexs = "466f726765742061626f7574207468652064617973207768656\
e2069742773206265656e20636c6f7564792e2042757420646f6e277420666f7267657420796f7572\
20686f75727320696e207468652073756e2e";
    char print_buff[1024];
    memset(print_buff, 0, 1024);

    ako_byte_buffer* buff1 = ako_bytebuf_new_copy((uint8_t*)const_chars, strlen(const_chars));
    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ(const_chars, print_buff);

    ako_bytebuf_print_hex(buff1, print_buff);
    EXPECT_STREQ(const_hexs, print_buff);

    ako_byte_buffer* buff2 = ako_bytebuf_new_wrap((uint8_t*)print_buff, strlen(print_buff));
    char* tmp_buff = (char*)ako_bytebuf_get_bytes(buff2, 10);
    char tmp_char[11];
    tmp_char[10] = 0;
    memcpy(tmp_char, tmp_buff, 10);
    EXPECT_NE(0, (uint64_t)tmp_buff);
    EXPECT_STREQ("466f726765", tmp_char);
    free(tmp_buff);

    ako_byte_buffer* buff3 = ako_bytebuf_new(2048);
    ako_bytebuf_put_bytebuf(buff3, buff1);
    ako_byte_buffer* buff5 = ako_bytebuf_resize(buff3, 10);
    EXPECT_EQ(0, (uint64_t)buff5);

    buff5 = ako_bytebuf_resize_force(buff3, 12);
    EXPECT_NE(0, (uint64_t)buff5);

    ako_bytebuf_print_ascii(buff3, print_buff);
    EXPECT_STREQ("Forget about", print_buff);
    EXPECT_TRUE(ako_bytebuf_is_full(buff3));
    EXPECT_FALSE(ako_bytebuf_is_empty(buff3));

    buff5 = ako_bytebuf_resize(buff3, 18);
    EXPECT_NE(0, (uint64_t)buff5);
    EXPECT_EQ(18, ako_bytebuf_size(buff3));
    EXPECT_EQ(12, ako_bytebuf_length(buff3));
    EXPECT_EQ(6, ako_bytebuf_valid(buff3));

    ako_byte_buffer* buff4 = ako_bytebuf_new_default();
    EXPECT_TRUE(ako_bytebuf_is_empty(buff4));
    EXPECT_FALSE(ako_bytebuf_is_full(buff4));

    ako_bytebuf_free(buff4);
    ako_bytebuf_free(buff3);
    ako_bytebuf_free(buff2);
    ako_bytebuf_free(buff1);
}

TEST(test_ako_bytebuffer, case_bytebuffer_boundary_condition_test_1) {
    int ret = 0;
    const char* const_chars = "Forget about the days when it's been cloudy. But \
don't forget your hours in the sun.";;
    char print_buff[1024];
    memset(print_buff, 0, 1024);

    ako_byte_buffer* buff1 = ako_bytebuf_new_default();
    ako_byte_buffer* buff2 = ako_bytebuf_resize_force(buff1, 64);
    EXPECT_TRUE(buff2);

    ako_bytebuf_skip(buff1, 10086);
    EXPECT_EQ(0, ako_bytebuf_pos(buff1));

    ako_bytebuf_put_bytes(buff1, (uint8_t*)const_chars, strlen(const_chars));
    ako_bytebuf_skip(buff1, 10086);
    EXPECT_EQ(64, ako_bytebuf_pos(buff1));

    buff2 = ako_bytebuf_clone(buff1);
    EXPECT_TRUE(buff2);
    EXPECT_TRUE(ako_bytebuf_equals(buff1, buff2));

    ret = ako_bytebuf_clear(buff2);
    EXPECT_TRUE(ret);
    EXPECT_EQ(0, ako_bytebuf_pos(buff2));
    EXPECT_EQ(0, ako_bytebuf_length(buff2));
    EXPECT_EQ(64, ako_bytebuf_size(buff2));

    ako_byte_buffer* buff3 = ako_bytebuf_resize_force(buff1, 12);
    ako_bytebuf_put_bytebuf(buff2, buff1);
    EXPECT_TRUE(buff3);
    buff3 = ako_bytebuf_resize_force(buff2, 12);
    EXPECT_TRUE(buff3);
    EXPECT_TRUE(ako_bytebuf_equals(buff1, buff2));

    uint8_t ch = ako_bytebuf_peek(buff1);
    EXPECT_EQ('F', ch);
    ch = ako_bytebuf_get(buff1);
    EXPECT_EQ('F', ch);
    ch = ako_bytebuf_peek(buff1);
    EXPECT_EQ('o', ch);
    ch = ako_bytebuf_get(buff1);
    EXPECT_EQ('o', ch);
    ch = ako_bytebuf_peek(buff1);
    EXPECT_EQ('r', ch);
    ch = ako_bytebuf_get(buff1);
    EXPECT_EQ('r', ch);
    ch = ako_bytebuf_get_at(buff1, 10);
    EXPECT_EQ('u', ch);
    ch = ako_bytebuf_get_at(buff1, 11);
    EXPECT_EQ('t', ch);

    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Forget about", print_buff);

    ako_bytebuf_replace(buff1, 'o', 't', 1, false);
    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Ftrget abtut", print_buff);

    ako_bytebuf_replace(buff1, 't', 'G', 3, false);
    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("FtrgeG abGuG", print_buff);

    ako_bytebuf_replace(buff1, 'G', '=', 3, true);
    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Ftrge= abGuG", print_buff);

    ako_bytebuf_free(buff2);
    ako_bytebuf_free(buff1);
}

TEST(test_ako_bytebuffer, case_bytebuffer_boundary_condition_test_2) {
    int ret = 0;
    const char* const_chars = "Forget about the days when it's been cloudy. But \
don't forget your hours in the sun.";
    char print_buff[1024];
    memset(print_buff, 0, 1024);

    ako_byte_buffer* buff1 = ako_bytebuf_new_default();
    ako_bytebuf_put_bytes(buff1, (uint8_t*)const_chars, strlen(const_chars));
    ako_byte_buffer* buff2 = ako_bytebuf_resize_force(buff1, 32);
    EXPECT_TRUE(buff2);

    ako_bytebuf_get_bytes_in(buff1, (uint8_t*)print_buff, 26);
    EXPECT_STREQ("Forget about the days when", print_buff);

    memset(print_buff, 0, 1024);
    ako_bytebuf_get_bytes_at_in(buff1, 7, (uint8_t*)print_buff, 20);
    EXPECT_STREQ("about the days when ", print_buff);

    uint8_t* tmp_buf = ako_bytebuf_get_bytes_at(buff1, 20, 7);
    EXPECT_TRUE(tmp_buf);
    memset(print_buff, 0, 1024);
    memcpy(print_buff, tmp_buf, 20);
    EXPECT_STREQ("about the days when ", print_buff);

    ako_bytebuf_resize(buff1, 40);
    ako_bytebuf_put(buff1, 'X');
    memset(print_buff, 0, 1024);
    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Forget about the days when it's X", print_buff);
    
    ako_bytebuf_put_at(buff1, 'Z', 0);
    memset(print_buff, 0, 1024);
    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Zorget about the days when it's X", print_buff);

    ako_bytebuf_put_bytes(buff1, (uint8_t*)"FK", 2);
    memset(print_buff, 0, 1024);
    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Zorget about the days when it's XFK", print_buff);

    ako_bytebuf_put_bytes_at(buff1, (uint8_t*)"CUCKOO", 6, 7);
    memset(print_buff, 0, 1024);
    ako_bytebuf_print_ascii(buff1, print_buff);
    EXPECT_STREQ("Zorget CUCKOOthe days when it's XFK", print_buff);

    ako_bytebuf_free(buff1);
}

TEST(test_ako_bytebuffer, case_bytebuffer_boundary_condition_test_3) {
    int ret = 0;
    char print_buff[1024];
    memset(print_buff, 0, 1024);

    ako_byte_buffer* buff1 = ako_bytebuf_new_default();
    ako_byte_buffer* buff2 = ako_bytebuf_resize_force(buff1, 40);
    EXPECT_TRUE(buff2);
    EXPECT_EQ(0, ako_bytebuf_pos(buff1));
    EXPECT_EQ(0, ako_bytebuf_length(buff1));
    EXPECT_EQ(40, ako_bytebuf_size(buff1));

    ako_bytebuf_put_double(buff1, 101005.01);
    EXPECT_EQ(sizeof(double), ako_bytebuf_length(buff1));
    ako_bytebuf_put_float(buff1, (float)102.02);
    EXPECT_EQ(sizeof(double) + sizeof(float), ako_bytebuf_length(buff1));
    ako_bytebuf_put_int(buff1, 103);
    EXPECT_EQ(sizeof(double) + sizeof(float) + sizeof(uint32_t), ako_bytebuf_length(buff1));
    ako_bytebuf_put_long(buff1, 10086);
    EXPECT_EQ(sizeof(double) + sizeof(float) + sizeof(uint32_t) + sizeof(uint64_t),
        ako_bytebuf_length(buff1));
    ako_bytebuf_put_short(buff1, 265);
    EXPECT_EQ(sizeof(double) + sizeof(float) + sizeof(uint32_t) + sizeof(uint64_t) 
        + sizeof(uint16_t), ako_bytebuf_length(buff1));

    ako_bytebuf_reset(buff1);
    EXPECT_NEAR(101005.01, ako_bytebuf_get_double(buff1), 0.1);
    EXPECT_NEAR(102.02, ako_bytebuf_get_float(buff1), 0.1);
    EXPECT_EQ(103, ako_bytebuf_get_int(buff1));
    EXPECT_EQ(10086, ako_bytebuf_get_long(buff1));
    EXPECT_EQ(265, ako_bytebuf_get_short(buff1));

    ako_bytebuf_put_double_at(buff1, 23628.2345, 1);
    ako_bytebuf_put_float_at(buff1, (float)348.324, 9);
    ako_bytebuf_put_int_at(buff1, 6745, 18);
    ako_bytebuf_put_long_at(buff1, 564313, 27);
    ako_bytebuf_put_short_at(buff1, 23456, 36);

    EXPECT_NEAR(23628.2345, ako_bytebuf_get_double_at(buff1, 1), 0.1);
    EXPECT_NEAR(348.324, ako_bytebuf_get_float_at(buff1, 9), 0.1);
    EXPECT_EQ(6745, ako_bytebuf_get_int_at(buff1, 18));
    EXPECT_EQ(564313, ako_bytebuf_get_long_at(buff1, 27));
    EXPECT_EQ(23456, ako_bytebuf_get_short_at(buff1, 36));

    ako_bytebuf_free(buff1);
}

#endif // AKOFS_TEST_AKO_BYTE_BUFFER_HPP_
