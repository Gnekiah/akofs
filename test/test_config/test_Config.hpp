#ifndef SPARKLE_TEST_CONFIG_HPP_
#define SPARKLE_TEST_CONFIG_HPP_

#include <gtest/gtest.h>
#include <exception>
#include <Config.h>

std::string ini_path = "../test/test_config/ini_tests/";

TEST(test_Config, case_1) {
    try {
        spk::Config* config = new spk::Config("");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = ": Permission denied or not exist.\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_2) {
    try {
        spk::Config *config = new spk::Config(ini_path + "bad_comment.ini");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = ini_path + "bad_comment.ini" + ": INI Syntax error.\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_3) {
    try {
        spk::Config* config = new spk::Config(ini_path + "bad_multi.ini");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = ini_path + "bad_multi.ini" + ": INI Syntax error.\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_4) {
    try {
        spk::Config* config = new spk::Config(ini_path + "bad_section.ini");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = ini_path + "bad_section.ini" + ": INI Syntax error.\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_5) {
    try {
        spk::Config* config = new spk::Config(ini_path + "bom.ini");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = "Error! Unknown the symbols:\nbom_section=bom_name = \
bom_value\nbom_section=key\xE2\x80\x9C = value\xE2\x80\x9C\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_6) {
    try {
        spk::Config* config = new spk::Config(ini_path + "duplicate_sections.ini");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = "Error! Unknown the symbols:\nsection1=single1 = \
abc\ndef\nsection1=single2 = xyz\nqrs\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_7) {
    try {
        spk::Config* config = new spk::Config(ini_path + "multi_line.ini");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = "Error! Unknown the symbols:\nsection1=multi = \
this is a\nmulti-line value\nsection1=single1 = abc\nsection1=single2 = \
xyz\nsection2=multi = a\nb\nc\nsection3=multi = the quick\nbrown fox\nsection3=name = \
bob smith\nsection3=single = ghi\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_8) {
    try {
        spk::Config* config = new spk::Config(ini_path + "no_value.ini");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = ini_path + "no_value.ini" + ": INI Syntax error.\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_9) {
    try {
        spk::Config* config = new spk::Config(ini_path + "user_error.ini");
    }
    catch (const std::runtime_error& e) {
        std::string exc_msg = "Error! Unknown the symbols:\nsection=a = \
b\nsection=c = d\nsection=user = parse_error\n";
        EXPECT_STREQ(exc_msg.c_str(), e.what());
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
}

TEST(test_Config, case_10) {
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

    spk::Config* config = nullptr;
    try {
        config = spk_init_config(ini_path + "normal.ini", &config_map);
        if (config == nullptr)
            throw -1;
    }
    catch (...) {
        ASSERT_TRUE(false);
    }
    EXPECT_STREQ("This is a test", config->Get("section1", "one", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ((long)1234, config->Get("section1", "two", SPK_CONFIG_TYPE_INT));
    EXPECT_EQ(4, config->Get(" section 2 ", "happy", SPK_CONFIG_TYPE_INT));
    EXPECT_STREQ("", config->Get(" section 2 ", "sad", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("1;2;3", config->Get("comment_test", "test1", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("2;3;4;this won't be a comment, needs whitespace before ';'",
        config->Get("comment_test", "test2", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ(345, config->Get("comment_test", "test;3", SPK_CONFIG_TYPE_INT));
    EXPECT_STREQ("4#5#6", config->Get("comment_test", "test4", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("", config->Get("comment_test", "test7", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("; not a comment, needs whitespace before ';'", 
        config->Get("comment_test", "test8", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("text/html", config->Get("colon_tests", "content-type", 
        SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("bar", config->Get("colon_tests", "foo", SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_EQ(42, config->Get("colon_tests", "adams", SPK_CONFIG_TYPE_INT));
    EXPECT_STREQ("with = equals", config->Get("colon_tests", "funny1", 
        SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("with : colons", config->Get("colon_tests", "funny2", 
        SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("two = equals", config->Get("colon_tests", "funny3", 
        SPK_CONFIG_TYPE_STRING).c_str());
    EXPECT_STREQ("two : colons", config->Get("colon_tests", "funny4", 
        SPK_CONFIG_TYPE_STRING).c_str());

    config->Set("section1", "one", true);
    EXPECT_TRUE(config->Get("section1", "one", SPK_CONFIG_TYPE_BOOL));
    config->Set("section1", "one", false);
    EXPECT_FALSE(config->Get("section1", "one", SPK_CONFIG_TYPE_BOOL));
    config->Set("section1", "one", 1);
    EXPECT_EQ(1, config->Get("section1", "one", SPK_CONFIG_TYPE_INT));
    config->Set("section1", "one", 1.1);
    EXPECT_EQ(1.1, config->Get("section1", "one", SPK_CONFIG_TYPE_DOUBLE));
    config->Set("section1", "one", "12345");
    EXPECT_STREQ("12345", config->Get("section1", "one", SPK_CONFIG_TYPE_STRING).c_str());
}

#endif // SPARKLE_TEST_CONFIG_HPP_
