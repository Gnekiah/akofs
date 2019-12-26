#ifndef SPARKLE_TEST_SPK_CONFIG_HPP_
#define SPARKLE_TEST_SPK_CONFIG_HPP_

#include <gtest/gtest.h>
#include <exception>
#include <spk_config.h>

TEST(test_spk_config, case_1) {
    std::map<std::string, std::string> config_map;
    config_map[std::string("section1=one")] = "This is a test";
    config_map[std::string("section1=two")] = "1234";
    config_map[std::string(" section 2 =happy")] = "4";
    config_map[std::string(" section 2 =sad")] = "";
    config_map[std::string("comment_test=test1")] = "1;2;3";
    config_map[std::string("comment_test=test2")] = "2;3;4;this won't be a comment, needs \
whitespace before ';'";
    config_map[std::string("comment_test=test;3")] = "345";
    config_map[std::string("comment_test=test4")] = "4#5#6";
    config_map[std::string("comment_test=test7")] = "";
    config_map[std::string("comment_test=test8")] = "; not a comment, needs whitespace before ';'";
    config_map[std::string("colon_tests=content-type")] = "text/html";
    config_map[std::string("colon_tests=foo")] = "bar";
    config_map[std::string("colon_tests=adams")] = "42";
    config_map[std::string("colon_tests=funny1")] = " = with = equals";
    config_map[std::string("colon_tests=funny2")] = "with : colons";
    config_map[std::string("colon_tests=funny3")] = "two = equals";
    config_map[std::string("colon_tests=funny4")] = "two : colons";

    spk_config_exit();

    std::string ini_path = "../test/test_config/ini_tests/";
    int ret = spk_config_init(ini_path + "normal.ini", &config_map);
    EXPECT_EQ(0, ret);

    EXPECT_STREQ("This is a test", 
        spk_config_get("section1", "one", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ((long)1234, 
        spk_config_get("section1", "two", SPK_CONFIG_TYPE_INT));
    EXPECT_EQ(4, 
        spk_config_get(" section 2 ", "happy", SPK_CONFIG_TYPE_INT));
    EXPECT_STREQ("", 
        spk_config_get(" section 2 ", "sad", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("1;2;3", 
        spk_config_get("comment_test", "test1", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("2;3;4;this won't be a comment, needs whitespace before ';'",
        spk_config_get("comment_test", "test2", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ(345, 
        spk_config_get("comment_test", "test;3", SPK_CONFIG_TYPE_INT));
    EXPECT_STREQ("4#5#6", 
        spk_config_get("comment_test", "test4", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("", 
        spk_config_get("comment_test", "test7", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("; not a comment, needs whitespace before ';'",
        spk_config_get("comment_test", "test8", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("text/html", 
        spk_config_get("colon_tests", "content-type",
        SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("bar", 
        spk_config_get("colon_tests", "foo", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ(42,
        spk_config_get("colon_tests", "adams", SPK_CONFIG_TYPE_INT));
    EXPECT_STREQ("with = equals",
        spk_config_get("colon_tests", "funny1",
        SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("with : colons", 
        spk_config_get("colon_tests", "funny2",
        SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("two = equals", 
        spk_config_get("colon_tests", "funny3",
        SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("two : colons", 
        spk_config_get("colon_tests", "funny4",
        SPK_CONFIG_TYPE_STRING).c_str());

    spk_config_set("section1", "one", true);
    EXPECT_TRUE(spk_config_get("section1", "one", SPK_CONFIG_TYPE_BOOL));
    spk_config_set("section1", "one", false);
    EXPECT_FALSE(spk_config_get("section1", "one", SPK_CONFIG_TYPE_BOOL));
    spk_config_set("section1", "one", 1);
    EXPECT_EQ(1, spk_config_get("section1", "one", SPK_CONFIG_TYPE_INT));
    spk_config_set("section1", "one", 1.1);
    EXPECT_EQ(1.1, spk_config_get("section1", "one", SPK_CONFIG_TYPE_DOUBLE));
    spk_config_set("section1", "one", "12345");
    EXPECT_STREQ("12345",
        spk_config_get("section1", "one", SPK_CONFIG_TYPE_STRING).c_str());
}

/*
 * TODO: 
 *      test for spk_config_get_##alias
 *      test for spk_config_set_##alias
 */

#endif // SPARKLE_TEST_SPK_CONFIG_HPP_
