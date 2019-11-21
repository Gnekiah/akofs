#ifndef SPARKLE_TEST_SPK_SET_PROCTITLE_H_
#define SPARKLE_TEST_SPK_SET_PROCTITLE_H_

#include <spk_setproctitle.h>

const int test_spk_setporoctitle_case_1(char** argv, const char* title) {
    int err = spk_setproctitle(argv, title);
    return err;
}

#endif // SPARKLE_TEST_SPK_SET_PROCTITLE_H_