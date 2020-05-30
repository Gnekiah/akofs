#ifndef AKOFS_TEST_AKO_COMPRESS_HPP_
#define AKOFS_TEST_AKO_COMPRESS_HPP_

#include <gtest/gtest.h>
#include <ako_compto.h>
#include <cstdlib>

TEST(test_ako_compress, case_compress_demo) {
    const char* buff = "hello world!";
    uint64_t buff_size = 13;
    uint64_t dst_size = ako_compress_bound(buff_size);
    char* buff_dst = (char*)malloc(sizeof(char) * dst_size);

    int ret = ako_compress((uint8_t*)buff_dst, &dst_size, (uint8_t*)buff, buff_size);
    ASSERT_EQ(0, ret);

    uint64_t un_dst_size = 20;
    char* un_buff_dst = (char*)malloc(sizeof(char) * un_dst_size);
    ret = ako_uncompress((uint8_t*)un_buff_dst, &un_dst_size, (uint8_t*)buff_dst, dst_size);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(un_dst_size, buff_size);
    EXPECT_STREQ(un_buff_dst, buff);

    free(buff_dst);
    free(un_buff_dst);
}

TEST(test_ako_compress, case_compress_big_area) {
    uint64_t buff_size = 64 * 1024;
    char* buff = (char*)malloc(sizeof(char) * buff_size);
    int* ptr = (int*)buff;
    while ((void*)ptr < (void*)(buff + buff_size)) {
        (*ptr) = rand();
        ptr++;
    }
    
    uint64_t dst_size = ako_compress_bound(buff_size);
    char* buff_dst = (char*)malloc(sizeof(char) * dst_size);

    int ret = ako_compress((uint8_t*)buff_dst, &dst_size, (uint8_t*)buff, buff_size);
    ASSERT_EQ(0, ret);

    uint64_t un_dst_size = buff_size;
    char* un_buff_dst = (char*)malloc(sizeof(char) * un_dst_size);
    ret = ako_uncompress((uint8_t*)un_buff_dst, &un_dst_size, (uint8_t*)buff_dst, dst_size);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(un_dst_size, buff_size);

    ptr = (int*)buff;
    int* ptr2 = (int*)un_buff_dst;
    while ((void*)ptr < (void*)(buff + buff_size)) {
        ASSERT_EQ(*ptr, *ptr2);
        ptr++;
        ptr2++;
    }

    free(buff);
    free(buff_dst);
    free(un_buff_dst);
}

#endif // AKOFS_TEST_AKO_COMPRESS_HPP_
