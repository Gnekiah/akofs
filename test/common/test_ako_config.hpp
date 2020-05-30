#ifndef AKOFS_TEST_AKO_CONFIG_HPP_
#define AKOFS_TEST_AKO_CONFIG_HPP_

#include <gtest/gtest.h>
#include <exception>
#include <ako_config.h>

TEST(test_ako_config, case_1) {
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

    ako_config_exit();

    std::string ini_path = "../test/common/ini_tests/";
    int ret = ako_config_init(ini_path + "normal.ini", &config_map);
    EXPECT_EQ(0, ret);

    EXPECT_STREQ("This is a test", 
        ako_config_get("section1", "one", AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ((long)1234, 
        ako_config_get("section1", "two", AKO_CONFIG_TYPE_INT));
    EXPECT_EQ(4, 
        ako_config_get(" section 2 ", "happy", AKO_CONFIG_TYPE_INT));
    EXPECT_STREQ("", 
        ako_config_get(" section 2 ", "sad", AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("1;2;3", 
        ako_config_get("comment_test", "test1", AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("2;3;4;this won't be a comment, needs whitespace before ';'",
        ako_config_get("comment_test", "test2", AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ(345, 
        ako_config_get("comment_test", "test;3", AKO_CONFIG_TYPE_INT));
    EXPECT_STREQ("4#5#6", 
        ako_config_get("comment_test", "test4", AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("", 
        ako_config_get("comment_test", "test7", AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("; not a comment, needs whitespace before ';'",
        ako_config_get("comment_test", "test8", AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("text/html", 
        ako_config_get("colon_tests", "content-type",
        AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("bar", 
        ako_config_get("colon_tests", "foo", AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ(42,
        ako_config_get("colon_tests", "adams", AKO_CONFIG_TYPE_INT));
    EXPECT_STREQ("with = equals",
        ako_config_get("colon_tests", "funny1",
        AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("with : colons", 
        ako_config_get("colon_tests", "funny2",
        AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("two = equals", 
        ako_config_get("colon_tests", "funny3",
        AKO_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("two : colons", 
        ako_config_get("colon_tests", "funny4",
        AKO_CONFIG_TYPE_STRING).c_str());

    ako_config_set("section1", "one", true);
    EXPECT_TRUE(ako_config_get("section1", "one", AKO_CONFIG_TYPE_BOOL));
    ako_config_set("section1", "one", false);
    EXPECT_FALSE(ako_config_get("section1", "one", AKO_CONFIG_TYPE_BOOL));
    ako_config_set("section1", "one", 1);
    EXPECT_EQ(1, ako_config_get("section1", "one", AKO_CONFIG_TYPE_INT));
    ako_config_set("section1", "one", 1.1);
    EXPECT_EQ(1.1, ako_config_get("section1", "one", AKO_CONFIG_TYPE_DOUBLE));
    ako_config_set("section1", "one", "12345");
    EXPECT_STREQ("12345",
        ako_config_get("section1", "one", AKO_CONFIG_TYPE_STRING).c_str());
}

/*
 * TODO: 
 *      test for ako_config_get_##alias
 *      test for ako_config_set_##alias
 */

#endif // AKOFS_TEST_AKO_CONFIG_HPP_
