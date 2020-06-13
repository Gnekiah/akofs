#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "../include/logger.h"

#include "test_hashlib.hpp"
#include "test_timelib.hpp"
#include "test_logger.hpp"
#include "test_ssllib.hpp"
#include "test_socket_server.hpp"

logger::Logger *logger_ = nullptr;

///hashlib              accepted - 2019/03/21 14:14:00
///                     updated  - 2019/03/25 20:03:00
///                     updated  - 2019/03/27 15:38:00
TEST(test_hashlib, case_1) {
    std::string msg = "Hello, World!";
    std::string hashval = hashlib::Sha1::GetSha1(msg);
    ASSERT_STREQ(TestSha1Case1(msg).c_str(), hashval.c_str());
    ASSERT_FALSE(TestSha1Case2(msg.c_str(), msg.length()));
}

///hashlib - XorMatric  accepted - 2019/03/27 16:28:00
TEST(test_hashlib, case_2) {
    std::string str1 = "b69e597b4e56e93b6c633767d159cb192f5a5dae";
    std::string str2 = "b69e597b4e56e92b6c633767d159cb192f5a5daf";
    std::string res;
    std::string ref  = "0000000000000010000000000000000000000001";
    ASSERT_EQ(TestSha1Case3(str1, str2, res), 0);
    ASSERT_STREQ(res.c_str(), ref.c_str());
}

///hashlib - IsEqual    accepted - 2019/04/01 15:05:00
TEST(test_hashlib, case_3) {
    std::string str1 = "hello";
    std::string str2 = "hello";
    std::string str3 = "world";
    ASSERT_EQ(TestSha1Case4(str1, str2), 1);
    ASSERT_EQ(TestSha1Case4(str1, str3), 0);
}

///timelib              accepted - 2019/03/25 14:57:00
TEST(test_timelib, case_1) {
    ASSERT_GT(TestTimeCase1(), 500000);
    ASSERT_LT(TestTimeCase1(), 510000);
    ASSERT_GT(TestTimeCase2(), 500000);
    ASSERT_LT(TestTimeCase2(), 510000);
    ASSERT_FALSE(TestTimeCase3());
    ASSERT_TRUE(TestTimeCase4());
}

///logger               accepted - 2019/03/25 19:34:00
TEST(test_logger, case_1) {
    ASSERT_FALSE(TestLoggerCase1(logger_));
}

///ssllib               accepted - 2019/03/26 19:26:00
TEST(test_ssllib, case_1) {
    std::string msg = "Test for signafication and cryption!";
    ASSERT_STREQ(TestRsaPairCase1(logger_, msg).c_str(), msg.c_str());
    ASSERT_STREQ(TestRsaPairCase2(logger_, msg).c_str(), msg.c_str());
    ASSERT_STREQ(TestRsaPairCase3(logger_, msg).c_str(), msg.c_str());
    ASSERT_STREQ(TestRsaPairCase4(logger_, msg).c_str(), msg.c_str());
    ASSERT_STREQ(TestRsaPairCase5(logger_, msg).c_str(), msg.c_str());
}

TEST(test_socket_server, case_1) {
    ASSERT_EQ(TestSocketServerCase1(logger_), 0);
}


int main(int argc, char **argv) {
    std::string log_path = (boost::filesystem::temp_directory_path() / "mandis.log").string();
    logger_ = new logger::Logger(log_path, LOG_LEVEL_TRACE);
    logger_->Start();;
    
    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();

    getchar();
    logger_->Stop();
    logger_->Join();

    return ret;
}