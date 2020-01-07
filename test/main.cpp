#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
/* for fix `error C2375`: redefinition errors in WinSock2.h */
#define WIN32_LEAN_AND_MEAN
#endif

#include <gtest/gtest.h>

#include "test_arch/test_spk_setproctitle.hpp"
#include "test_arch/test_spk_compat.hpp"
#include "test_config/test_Config.hpp"
#include "test_config/test_spk_config.hpp"
#include "test_log/test_spk_logger.hpp"
#include "test_compto/test_spk_crc32.hpp"
#include "test_compto/test_spk_compress.hpp"
#include "test_compto/test_spk_hash.hpp"
#include "test_compto/test_spk_base64.hpp"
#include "test_compto/test_spk_des.hpp"
#include "test_compto/test_spk_rsa.hpp"
#include "test_compto/test_spk_aes.hpp"
#include "test_common/test_CircularBuffer.hpp"
#include "test_common/test_spk_bytebuffer.hpp"
#include "test_common/test_ByteBuffer.hpp"
#include "test_common/test_spk_slab.hpp"
#include "test_arch/test_spk_builtin.hpp"
#include "test_eventd/test_libuv_socket.hpp"

#include "test_common/test_spk_getopt.hpp"

int main(int argc, char** argv) {
#ifdef TEST_SPK_SET_PROCTITLE_DEF_ARGV
    test_arch_test_argc = argc;
    test_arch_test_argv = argv;
#endif

    testing::InitGoogleTest(&argc, argv);
    int err = RUN_ALL_TESTS();

    return err;
}
