#include "../include/timelib.h"

#include <sstream>
#include <boost/thread.hpp>

namespace timelib {
    Time::Time(int utc)
        : utc_(utc)
    {
        origin_time_ = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
        set_timestr_format("yyyy-MM-dd hh:mm:ss.zzzzzz");
    }

    Time::Time(const char* timestr_format, int utc)
        : utc_(utc)
    {
        origin_time_ = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
        set_timestr_format(timestr_format);
    }

    void Time::MilliSleep(uint64_t msec) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(msec));
    }

    void Time::MicroSleep(uint64_t msec) {
        boost::this_thread::sleep(boost::posix_time::microseconds(msec));
    }

    uint64_t Time::GetMicroTimeStamp() {
        now_ = boost::posix_time::microsec_clock::local_time();
        diff_ = now_ - origin_time_;
        return diff_.total_microseconds() - utc_ * 3600000000;
    }

    std::string Time::GetMicroTimeStr() {
        return MicroTimeStamp2Str(GetMicroTimeStamp());
    }

    uint64_t Time::MicroStr2TimeStamp(const char* timestr) {
        boost::posix_time::ptime pt = boost::posix_time::time_from_string(timestr);
        diff_ = pt - origin_time_;
        return diff_.total_microseconds() - utc_ * 3600000000;
    }

    std::string Time::MicroTimeStamp2Str(uint64_t ts) {
        ts += utc_ * 3600000000;
        int ms = ts % 1000000;
        ts = ts / 1000000;
        std::stringstream buff;
        boost::posix_time::ptime pt(boost::posix_time::from_time_t(ts));
        std::string timestr = boost::posix_time::to_iso_extended_string(pt);
        buff << timestr.substr(0, 10) << " " << timestr.substr(11);
        buff << "." << std::setw(6) << std::setfill('0') << ms;
        return buff.str();
    }
}