#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <gtest/gtest.h>

#include "test_arch/test_spk_setproctitle.hpp"
#include "test_arch/test_spk_compat.hpp"
#include "test_config/test_Config.hpp"
#include "test_log/test_spk_logger.hpp"
#include "test_compto/test_spk_crc32.hpp"
#include "test_compto/test_spk_compress.hpp"
#include "test_compto/test_spk_hash.hpp"
#include "test_compto/test_spk_base64.hpp"
#include "test_compto/test_spk_des.hpp"
#include "test_compto/test_spk_rsa.hpp"
#include "test_compto/test_spk_aes.hpp"
#include "test_common/test_CircularBuffer.hpp"

int main(int argc, char** argv) {
    test_arch_test_argc = argc;
    test_arch_test_argv = argv;

    testing::InitGoogleTest(&argc, argv);
    int err = RUN_ALL_TESTS();

    return err;
}
