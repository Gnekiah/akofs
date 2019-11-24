/*
 * Configuration interface.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_CONFIG_H_
#define SPARKLE_CONFIG_H_

#include <string>
#include <map>
#include <iostream>
#include <spk_macros.h>

class INIReader;

typedef int* spk_config_int_t;
typedef char spk_config_bool_t;
typedef double spk_config_double_t;
typedef std::string spk_config_string_t;

#define SPK_CONFIG_TYPE_INT     nullptr
#define SPK_CONFIG_TYPE_BOOL    '0'
#define SPK_CONFIG_TYPE_DOUBLE  1.1
#define SPK_CONFIG_TYPE_STRING  ""

namespace spk {

    class Config {
    public:
        Config() = delete;
        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;
        explicit Config(const std::string& path, std::map<std::string, std::string> *config_map = nullptr);
        // explicit Config(const std::string& path, std::string delimiter = "=", std::string comment = "#");
        ~Config();

        void Set(const std::string& section, const std::string& name, const int value);
        void Set(const std::string& section, const std::string& name, const long value);
        void Set(const std::string& section, const std::string& name, const double value);
        void Set(const std::string& section, const std::string& name, const bool value);
        void Set(const std::string& section, const std::string& name, const std::string& value);

        long Get(const std::string& section, const std::string& name, const spk_config_int_t);
        double Get(const std::string& section, const std::string& name, const spk_config_double_t);
        bool Get(const std::string& section, const std::string& name, const spk_config_bool_t);
        std::string Get(const std::string& section, const std::string& name, const spk_config_string_t);

        // template<class T>
        // T Set(const std::string& section, const std::string& name, T& value);
        // template<class T>
        // T Get(const std::string& section, const std::string& name);

        // bool Has(const std::string& section, const std::string& name) const;

        // std::string delimiter() const { return delimiter_; }
        // std::string comment() const { return comment_; }
        // void set_delimiter(const std::string& delimiter) { delimiter_ = delimiter; }
        // void set_comment(const std::string& comment) { comment_ = comment; }

        // friend std::ostream& operator << (std::ostream& os, const Config& config);
        // friend std::istream& operator >> (std::istream& is, Config& config);
       
    private:
        void InitConfig();

    private:
        // std::string delimiter_;
        // std::string comment_;
        // std::map<std::string, std::string> contents_;
        // INIReader *inireader_ = nullptr;

        std::map<std::string, std::string> *config_map_ = nullptr;
    };
}

#endif  // SPARKLE_CONFIG_H_
