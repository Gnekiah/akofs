#ifndef MANDIS_LOGGER_H_
#define MANDIS_LOGGER_H_

#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>

#include "timelib.h"

#define LOG_LEVEL_OFF           0
#define LOG_LEVEL_ERROR         1
#define LOG_LEVEL_WARNING       2
#define LOG_LEVEL_INFO          3
#define LOG_LEVEL_DEBUG         4
#define LOG_LEVEL_TRACE         5

#define LOG_ERROR(handler, stream)      handler->Logging(stream, __FUNCTION__, LOG_LEVEL_ERROR)
#define LOG_WARNING(handler, stream)    handler->Logging(stream, __FUNCTION__, LOG_LEVEL_WARNING)
#define LOG_INFO(handler, stream)       handler->Logging(stream, __FUNCTION__, LOG_LEVEL_INFO)
#define LOG_DEBUG(handler, stream)      handler->Logging(stream, __FUNCTION__, LOG_LEVEL_DEBUG)
#define LOG_TRACE(handler, stream)      handler->Logging(stream, __FUNCTION__, LOG_LEVEL_TRACE)

namespace logger {
    typedef struct LogField {
        uint64_t    timestamp;
        int64_t     level;
        char        caller[64];
        char        logmsg[944];
    } LogField;     /* 1024 bytes */
    
    class Logger {
    public:
        Logger(const std::string &log_path, int log_level=0);
        Logger(const char *log_path, int log_level = 0);
        ~Logger();

        void Logging(const std::stringstream &log, const char* caller, const int level);
        void Logging(const std::string &log, const char* caller, const int level);
        void Logging(const char *log, const char* caller, const int level);

        void Run();
        void Start();
        void Stop();
        void Join();

    private:
        std::string log_path_;
        int log_level_;
        timelib::Time time_;
        boost::lockfree::queue<LogField> *log_queue_ = nullptr;
        boost::thread thrd_;
    };


}

#endif // !MANDIS_LOGGER_H_

