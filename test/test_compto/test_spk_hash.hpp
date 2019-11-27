#ifndef SPARKLE_TEST_SPK_HASH_HPP_
#define SPARKLE_TEST_SPK_HASH_HPP_

#include <gtest/gtest.h>
#include <spk_compto.h>
#include <string>

TEST(test_spk_hash, case_md4_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64];
    uint8_t* ret = spk_md4((const uint8_t*)buf, size, md);
    ASSERT_EQ(md, ret);
}


TEST(test_spk_hash, case_md4_hex_check_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64] = { 0 };
    uint8_t* ret = spk_md4((const uint8_t*)buf, size, md);
    std::string encode = std::string((const char*)md);

    std::string encode_hex;
    char tmp[3] = { 0 };
    for (int i = 0; i < 16; i++)
    {
        sprintf(tmp, "%02x", md[i]);
        encode_hex += tmp;
    }
    EXPECT_STREQ("771574135e78bafe958c69ad3edfdcea", encode_hex.c_str());
}

TEST(test_spk_hash, case_md5_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64];
    uint8_t* ret = spk_md5((const uint8_t*)buf, size, md);
    ASSERT_EQ(md, ret);
}

TEST(test_spk_hash, case_md5_hex_check_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64] = { 0 };
    uint8_t* ret = spk_md5((const uint8_t*)buf, size, md);
    std::string encode = std::string((const char*)md);

    std::string encode_hex;
    char tmp[3] = { 0 };
    for (int i = 0; i < 16; i++)
    {
        sprintf(tmp, "%02x", md[i]);
        encode_hex += tmp;
    }
    EXPECT_STREQ("ba225c7364dbd54fdf9a244454029609", encode_hex.c_str());
}

TEST(test_spk_hash, case_mdc2_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64];
    uint8_t* ret = spk_mdc2((const uint8_t*)buf, size, md);
    ASSERT_EQ(md, ret);
}

TEST(test_spk_hash, case_mdc2_hex_check_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64] = { 0 };
    uint8_t* ret = spk_mdc2((const uint8_t*)buf, size, md);
    std::string encode = std::string((const char*)md);

    std::string encode_hex;
    char tmp[3] = { 0 };
    for (int i = 0; i < 16; i++)
    {
        sprintf(tmp, "%02x", md[i]);
        encode_hex += tmp;
    }
    EXPECT_STREQ("a411d52e316cd92c67a442e847cae3c8", encode_hex.c_str());
}

TEST(test_spk_hash, case_sha1_hex_check_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64] = { 0 };
    uint8_t* ret = spk_sha1((const uint8_t*)buf, size, md);
    ASSERT_EQ(md, ret);

    std::string encode_hex;
    char tmp[3] = { 0 };
    for (int i = 0; i < 20; i++)
    {
        sprintf(tmp, "%02x", md[i]);
        encode_hex += tmp;
    }
    EXPECT_STREQ("f3f1b36131f0fd322520e21b2fbd09a60a54f0c1", encode_hex.c_str());
}

TEST(test_spk_hash, case_sha224_hex_check_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64] = { 0 };
    uint8_t* ret = spk_sha224((const uint8_t*)buf, size, md);
    ASSERT_EQ(md, ret);

    std::string encode_hex;
    char tmp[3] = { 0 };
    for (int i = 0; i < 28; i++)
    {
        sprintf(tmp, "%02x", md[i]);
        encode_hex += tmp;
    }
    EXPECT_STREQ("9632f72d317d346dac100766551f002f2490fd11c24b200fe708f0a0", 
        encode_hex.c_str());
}

TEST(test_spk_hash, case_sha256_hex_check_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64] = { 0 };
    uint8_t* ret = spk_sha256((const uint8_t*)buf, size, md);
    ASSERT_EQ(md, ret);

    std::string encode_hex;
    char tmp[3] = { 0 };
    for (int i = 0; i < 32; i++)
    {
        sprintf(tmp, "%02x", md[i]);
        encode_hex += tmp;
    }
    EXPECT_STREQ("c49a916590993e9d3f9e79c2b751c7fbfabfb7d10bda4a2b4dcca46906bbf916",
        encode_hex.c_str());
}

TEST(test_spk_hash, case_sha384_hex_check_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64] = { 0 };
    uint8_t* ret = spk_sha384((const uint8_t*)buf, size, md);
    ASSERT_EQ(md, ret);

    std::string encode_hex;
    char tmp[3] = { 0 };
    for (int i = 0; i < 48; i++)
    {
        sprintf(tmp, "%02x", md[i]);
        encode_hex += tmp;
    }
    EXPECT_STREQ("0f05e6d7e0d14eac7475a9add9aa1dc92d8e0758ac08bb11523cf5b92badff\
4c0088607aefd07f6d90a94b3d324fe1c3",
        encode_hex.c_str());
}

TEST(test_spk_hash, case_sha512_hex_check_1) {
    const char* buf = "this is a buffer";
    uint32_t size = 16;
    uint8_t md[64] = { 0 };
    uint8_t* ret = spk_sha512((const uint8_t*)buf, size, md);
    ASSERT_EQ(md, ret);

    std::string encode_hex;
    char tmp[3] = { 0 };
    for (int i = 0; i < 64; i++)
    {
        sprintf(tmp, "%02x", md[i]);
        encode_hex += tmp;
    }
    EXPECT_STREQ("7ec7996892f21856ca1fc084ed659243517694b7297970523ab37fd1a721c1b\
b660775bcce077c91fa17c8bbcf477b5afa21ffc34c04945ba5ff72676f2c0fe7",
encode_hex.c_str());
}

#endif // SPARKLE_TEST_SPK_HASH_HPP_
