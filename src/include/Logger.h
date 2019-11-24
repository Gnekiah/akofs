/*
 * Header definition for logger interface.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_LOG_LOGGER_H_
#define SPARKLE_LOG_LOGGER_H_

#include <string>

namespace spk {

    class Logger {
    public:
        Logger() = delete;
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        explicit Logger(std::string& config_path);
        // explicit Config(const std::string& path, std::string delimiter = "=", std::string comment = "#");
        ~Logger();

    public:
        static int CheckConfig(std::string& config_path);

    private:

    };
}

#endif  // SPARKLE_LOG_LOGGER_H_
