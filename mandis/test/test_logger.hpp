#ifndef MANDIS_TEST_LOGGER_HPP_
#define MANDIS_TEST_LOGGER_HPP_

#include "../include/logger.h"

bool TestLoggerCase1(logger::Logger *log) {
    LOG_ERROR(log, "Error");
    LOG_WARNING(log, "Warning");
    LOG_INFO(log, "Information");
    LOG_DEBUG(log, "Debug");
    LOG_TRACE(log, "Trace");
    return 0;
}


#endif // !MANDIS_TEST_LOGGER_HPP_

