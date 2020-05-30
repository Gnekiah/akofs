#ifndef AKOFS_TEST_AKO_LOGGER_HPP_
#define AKOFS_TEST_AKO_LOGGER_HPP_

#include <gtest/gtest.h>
#include <ako_logger.h>
#include <string>

#if defined(__linux__)
#include <unistd.h>
#define msleep(t) usleep(t * 1000)
#else
#include <Windows.h>
#define msleep(t) Sleep(t)
#endif 

std::string conf_path = "../test/common/conf_tests/";

TEST(test_ako_logger, case_1) {
    int ret = 0;
    ret = ako_logger_init((conf_path + "test_hex.conf").c_str());
    EXPECT_EQ(0, ret);
    ret = ako_logger_reload ((conf_path + "test_hex.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_3) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_longlog.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_4) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_profile.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_5) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_category.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_6) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_init.2.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_7) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_mdc.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_8) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_record.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_9) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_conf.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_10) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_init.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_11) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_multithread.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_12) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_syslog.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_13) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_default.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_14) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_leak.2.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_15) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_pipe.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_16) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_tmp.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_17) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_enabled.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_18) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_leak.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_19) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_press_zlog.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_20) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_hello.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_21) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_level.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_22) {
    int ret = 0;
    ret = ako_logger_reload((conf_path + "test_press_zlog2.conf").c_str());
    ASSERT_LE(-1, ret);
    ASSERT_GE(0, ret);
    msleep(10);
}

TEST(test_ako_logger, case_parse_conf_and_do_logging) {
    int ret = 0;
    msleep(500);
    ret = ako_logger_reload((conf_path + "normal.conf").c_str());
    ASSERT_EQ(0, ret);

    akolog_debug("akolog_debug");
    akolog_info("akolog_info");
    akolog_notice("akolog_notice");
    akolog_warn("akolog_warn");
    akolog_error("akolog_error");
    akolog_fatal("akolog_fatal");
    const char* buf = "this is a buffer";
    int buf_len = 16;
    akolog_hex_debug(buf, buf_len);
    akolog_hex_info(buf, buf_len);
    akolog_hex_notice(buf, buf_len);
    akolog_hex_warn(buf, buf_len);
    akolog_hex_error(buf, buf_len);
    akolog_hex_fatal(buf, buf_len);
    ako_logger_free();
}

#endif // !AKOFS_TEST_AKO_LOGGER_HPP_

