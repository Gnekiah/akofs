#include <gtest/gtest.h>

#include "test_arch/test_spk_setproctitle.hpp"

int test_argc;
char** test_argv;

int main(int argc, char** argv) {
    test_argc = argc;
    test_argv = argv;

    testing::InitGoogleTest(&argc, argv);
    int err = RUN_ALL_TESTS();

    return err;
}
