#ifndef AKOFS_TEST_AKO_SET_PROCTITLE_HPP_
#define AKOFS_TEST_AKO_SET_PROCTITLE_HPP_

#include <string>
#include <ako_setproctitle.h>

#define TEST_AKO_SET_PROCTITLE_DEF_ARGV
int test_arch_test_argc;
char** test_arch_test_argv;

const int test_ako_setporoctitle_case_1(char** argv, const char* title) {
    int err = ako_setproctitle(const_cast<const char**>(argv), title);
    return err;
}

TEST(test_ako_setproctitle, case_1) {
    std::string short_title = "ako";
    std::string middle_title = "sparkle_test";
    std::string long_title = "sparkle_test_sparkle_test_sparkle_test_sparkle_test_\
sparkle_test_sparkle_test_sparkle_test_sparkle_test_sparkle_test";
    ASSERT_EQ(test_ako_setporoctitle_case_1(test_arch_test_argv, short_title.c_str()), 0);
#ifdef __linux__
    ASSERT_STREQ(test_arch_test_argv[0], short_title.c_str());
#else
    ASSERT_STREQ(short_title.c_str(), short_title.c_str());
#endif
    ASSERT_EQ(test_ako_setporoctitle_case_1(test_arch_test_argv, long_title.c_str()), 0);
#ifdef __linux__
    ASSERT_STREQ(test_arch_test_argv[0], long_title.c_str());
#else
    ASSERT_STREQ(long_title.c_str(), long_title.c_str());
#endif
    ASSERT_EQ(test_ako_setporoctitle_case_1(test_arch_test_argv, middle_title.c_str()), 0);
#ifdef __linux__
    ASSERT_STREQ(test_arch_test_argv[0], middle_title.c_str());
#else
    ASSERT_STREQ(middle_title.c_str(), middle_title.c_str());
#endif
}

#endif // AKOFS_TEST_AKO_SET_PROCTITLE_HPP_
