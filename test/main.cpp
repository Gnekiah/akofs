#include <gtest/gtest.h>

#include "test_arch/test_spk_setproctitle.hpp"
#include "test_arch/test_spk_compat.hpp"
#include "test_config/test_Config.hpp"
#include "test_log/test_spk_logger.hpp"

int main(int argc, char** argv) {
    test_arch_test_argc = argc;
    test_arch_test_argv = argv;

    testing::InitGoogleTest(&argc, argv);
    int err = RUN_ALL_TESTS();

    return err;
}
