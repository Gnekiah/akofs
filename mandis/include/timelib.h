#ifndef MANDIS_TIMELIB_H_
#define MANDIS_TIMELIB_H_

#include <boost/date_time/posix_time/posix_time.hpp>

namespace timelib {
    class Time {
    public:
        Time(int utc=8);
        Time(const char* timestr_format, int utc=8);
        ~Time() { }

        static void MilliSleep(uint64_t msec);
        static void MicroSleep(uint64_t msec);

        uint64_t GetMicroTimeStamp();
        std::string GetMicroTimeStr();

        uint64_t MicroStr2TimeStamp(const char* timestr);
        std::string MicroTimeStamp2Str(uint64_t timestamp);

        void set_utc(int utc) { utc_ = utc; }
        void set_timestr_format(const char* timestr_format) { strcpy(timestr_format_, timestr_format); }

    private:
        int utc_;
        char timestr_format_[32];
        boost::posix_time::ptime origin_time_;
        boost::posix_time::ptime now_;
        boost::posix_time::time_duration diff_;
    };

}


#endif // !MANDIS_TIMELIB_H_

