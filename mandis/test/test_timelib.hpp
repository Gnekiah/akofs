#ifndef MANDIS_TEST_TIMELIB_HPP_
#define MANDIS_TEST_TIMELIB_HPP_

#include <iostream>
#include <string>

#include "../include/timelib.h"

uint64_t TestTimeCase1() {
    timelib::Time time;
    uint64_t ts = time.GetMicroTimeStamp();
    time.MilliSleep(500);
    return time.GetMicroTimeStamp() - ts;   
}

uint64_t TestTimeCase2() {
    timelib::Time time;
    uint64_t ts = time.GetMicroTimeStamp();
    time.MicroSleep(500000);
    return time.GetMicroTimeStamp() - ts;
}

bool TestTimeCase3() {
    timelib::Time time;
    std::string str1 = time.GetMicroTimeStr();
    uint64_t ts = time.MicroStr2TimeStamp(str1.c_str());
    std::string str2 = time.MicroTimeStamp2Str(ts);
    return str1.compare(str2);
}

bool TestTimeCase4() {
    timelib::Time time;
    uint64_t ts1 = time.GetMicroTimeStamp();
    std::string str1 = time.MicroTimeStamp2Str(ts1);
    uint64_t ts2 = time.MicroStr2TimeStamp(str1.c_str());
    return ts1 == ts2;
}

#endif // !MANDIS_TEST_TIMELIB_HPP_

