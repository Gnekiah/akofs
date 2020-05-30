#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
/* for fix `error C2375`: redefinition errors in WinSock2.h */
#define WIN32_LEAN_AND_MEAN
#endif

#include <gtest/gtest.h>

#include "arch/test_ako_setproctitle.hpp"
#include "arch/test_ako_compat.hpp"
#include "arch/test_ako_builtin.hpp"

#include "common/test_Config.hpp"
#include "common/test_ako_config.hpp"
#include "common/test_ako_logger.hpp"
#include "common/test_ako_crc32.hpp"
#include "common/test_ako_compress.hpp"
#include "common/test_ako_hash.hpp"
#include "common/test_ako_base64.hpp"
#include "common/test_ako_des.hpp"
#include "common/test_ako_rsa.hpp"
#include "common/test_ako_aes.hpp"
#include "common/test_CircularBuffer.hpp"
#include "common/test_ako_bytebuffer.hpp"
#include "common/test_ByteBuffer.hpp"
#include "common/test_ako_slab.hpp"
#include "common/test_ako_getopt.hpp"

#include "daemon/test_libuv_socket.hpp"


int main(int argc, char** argv) {
#ifdef TEST_AKO_SET_PROCTITLE_DEF_ARGV
    test_arch_test_argc = argc;
    test_arch_test_argv = argv;
#endif

    testing::InitGoogleTest(&argc, argv);
    int err = RUN_ALL_TESTS();

    return err;
}
