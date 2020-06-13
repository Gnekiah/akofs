#include "../include/logger.h"

#include <fstream>

namespace logger {
    Logger::Logger(const std::string &log_path, int log_level)
        : log_path_(log_path), log_level_(log_level) 
    {
        log_queue_ = new boost::lockfree::queue<LogField>(256);
    }

    Logger::Logger(const char *log_path, int log_level)
        : log_path_(log_path), log_level_(log_level) 
    {
        log_queue_ = new boost::lockfree::queue<LogField>(256);
    }

    Logger::~Logger() {
        delete log_queue_;
    }

    void Logger::Logging(const std::stringstream &log, const char* caller, const int level) {
        Logging(log.str().c_str(), caller, level);
    }

    void Logger::Logging(const std::string &log, const char* caller, const int level) {
        Logging(log.c_str(), caller, level);
    }

    void Logger::Logging(const char *log, const char* caller, const int level) {
        if (log_level_ < level)
            return;
        LogField log_field;
        memset(&log_field, 0, sizeof(log_field));
        log_field.timestamp = time_.GetMicroTimeStamp();
        log_field.level = level;
        strcpy(log_field.logmsg, log);
        strcpy(log_field.caller, caller);
        log_queue_->push(log_field);
    }

    void Logger::Run() {
        LogField log;
        std::stringstream log_stream;
        std::ofstream out;
        out.open(log_path_, std::ios::app);
        
        LOG_TRACE(this, "Logger Run!");
        while (true) {
            while (log_queue_->empty())
                time_.MilliSleep(2);    /* sleep for 2 ms */

            log_queue_->pop(log);
            log_stream << time_.MicroTimeStamp2Str(log.timestamp);
            switch (log.level)
            {
            case LOG_LEVEL_ERROR:
                log_stream << " [ERROR] <"; break;
            case LOG_LEVEL_WARNING:
                log_stream << " [WARN]  <"; break;
            case LOG_LEVEL_INFO:
                log_stream << " [INFO]  <"; break;
            case LOG_LEVEL_DEBUG:
                log_stream << " [DEBUG] <"; break;
            case LOG_LEVEL_TRACE:
                log_stream << " [TRACE] <"; break;
            default:
                assert(0); break;
            }
            log_stream << log.caller << "> " << log.logmsg << std::endl;
            out << log_stream.str().c_str();

#ifdef _ON_DEBUG
            std::cerr << log_stream.str();
#endif

            log_stream.str("");
            out.flush();
        }
        out.close();
    }

    void Logger::Start() {
        LOG_TRACE(this, "Logger Start!");
        thrd_ = boost::thread(boost::bind(&Logger::Run, this));
    }

    void Logger::Stop() {
        LOG_TRACE(this, "Logger Stop!");
        time_.MilliSleep(50);
        thrd_.interrupt();
    }

    void Logger::Join() {
        LOG_TRACE(this, "Logger Join!");
        thrd_.join();
    }
}