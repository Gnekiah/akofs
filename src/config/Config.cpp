/*
 * source of Config module
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <Config.hpp>

#include <sstream>
#include <algorithm>
#include <cassert>

#ifdef  _WIN32
#include <io.h>
#define __tmp__access__ _access 
#elif defined(__linux__)
#include <unistd.h>
#define __tmp__access__ access
#else
#error No <io.h> or <sys/io.h> found.
#endif //  _WIN32

#include "inih/INIReader.hpp"

spk::Config* g_spk_config = nullptr;

spk::Config* spk_config_init(const std::string& path,
    std::map<std::string, std::string>* config_map) {
    if (!g_spk_config) {
        try {
            g_spk_config = new spk::Config(path, config_map);
        }
        catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
    }
    return g_spk_config;
}

namespace spk {
    template<typename T>
    static inline std::string type2str(const T& t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    template<typename T>
    static inline T str2type(const std::string& s, T)
    {
        T t;
        std::stringstream ss(s);
        ss >> t;
        return t;
    }

    static inline std::string make_key(const std::string& section, const std::string& name)
    {
        std::string key = section + "=" + name;
        // Convert to lower case to make section/name lookups case-insensitive
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        return key;
    }

    Config::Config(const std::string& path, std::map<std::string, std::string>* config_map) {
        if (__tmp__access__(path.c_str(), 04)) {
            std::stringstream err_ss;
            err_ss << path << ": Permission denied or not exist." << std::endl;
            throw std::runtime_error(err_ss.str());
        }
        // load config from path
        INIReader inireader(path);
        int err = inireader.ParseError();
        if (err) {
            std::stringstream err_ss;
            err_ss << path << ": INI Syntax error." << std::endl;
            throw std::runtime_error(err_ss.str());
        }
        // init config_map_
        config_map_ = new std::map<std::string, std::string>();
        if (config_map) {
            for (auto iter = config_map->begin(); iter != config_map->end(); iter++) {
                config_map_->insert(std::pair<std::string, std::string>(iter->first, iter->second));
            }
        }

        // syntax recheck
        std::map<std::string, std::string> __unknown_map;
        std::map<std::string, std::string>& __config_map = inireader._values;
        for (auto iter = __config_map.begin(); iter != __config_map.end(); iter++) {
            std::string key = iter->first;
            std::string val = iter->second;
            if (config_map_->count(key))
                config_map_->find(key)->second = val;
            else // syntax error: unknown key
                __unknown_map.insert(std::pair<std::string, std::string>(key, val));
        }

        // print parse failed message
        if (__unknown_map.size()) {
            std::stringstream err_ss;
            err_ss << "Error! Unknown the symbols:" << std::endl;
            for (auto iter = __unknown_map.begin(); iter != __unknown_map.end(); iter++) {
                err_ss << iter->first << " = " << iter->second << std::endl;
            }
            throw std::runtime_error(err_ss.str());
        }
    }

    void Config::Set(const std::string& section, const std::string& name, const int value) {
        Set(section, name, type2str(value));
    }
    void Config::Set(const std::string& section, const std::string& name, const long value) {
        Set(section, name, type2str(value));
    }
    void Config::Set(const std::string& section, const std::string& name, const double value) {
        Set(section, name, type2str(value));
    }
    void Config::Set(const std::string& section, const std::string& name, const bool value) {
        Set(section, name, type2str(value));
    }
    void Config::Set(const std::string& section, const std::string& name, const char* value) {
        Set(section, name, type2str(value));
    }
    void Config::Set(const std::string& section, const std::string& name, const std::string& value) {
        int err = -1;
        std::string key = make_key(section, name);
        lock_.lock();
        if (config_map_->count(key)) {
            config_map_->find(key)->second = value;
            err = 0;
        }
        lock_.unlock();
        if (!err)
            return;
        std::cerr << "<" << __FUNCTION__ << "> Error! Unknown the key: " << key << std::endl;
        assert(false);
    }

    long Config::Get(const std::string& section, const std::string& name, spk_config_int_t) {
        const std::string val = Get(section, name, SPK_CONFIG_TYPE_STRING);
        return str2type(val, 1);
    }
    double Config::Get(const std::string& section, const std::string& name, spk_config_double_t) {
        const std::string val = Get(section, name, SPK_CONFIG_TYPE_STRING);
        return str2type(val, 0.1);
    }
    bool Config::Get(const std::string& section, const std::string& name, spk_config_bool_t) {
        const std::string val = Get(section, name, SPK_CONFIG_TYPE_STRING);
        return str2type(val, true);
    }
    std::string Config::Get(const std::string& section, const std::string& name, spk_config_string_t) {
        int err = -1;
        std::string key = make_key(section, name);
        std::string val;
        // Use _values.find() here instead of _values.at() to support pre C++11 compilers
        lock_.lock();
        if (config_map_->count(key)) {
            val = config_map_->find(key)->second;
            err = 0;
        }
        lock_.unlock();
        if (!err)
            return val;
        std::cerr << "<" << __FUNCTION__ << "> Error! Unknown the key: " << key << std::endl;
        assert(false);
    }
}
