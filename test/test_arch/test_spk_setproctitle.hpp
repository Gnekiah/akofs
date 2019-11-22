#ifndef SPARKLE_TEST_SPK_SET_PROCTITLE_H_
#define SPARKLE_TEST_SPK_SET_PROCTITLE_H_

#include <spk_setproctitle.h>

int test_arch_test_argc;
char** test_arch_test_argv;

const int test_spk_setporoctitle_case_1(char** argv, const char* title) {
    int err = spk_setproctitle(argv, title);
    return err;
}

TEST(test_spk_setproctitle, case_1) {
    ASSERT_EQ(test_spk_setporoctitle_case_1(test_arch_test_argv, "sparkle_test"), 0);
}

#endif // SPARKLE_TEST_SPK_SET_PROCTITLE_H_