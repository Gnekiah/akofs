/*
 * Configuration interface.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include "Config.hpp"
#include <ako_config.h>
#include <ako_compat.h>

static ako::Config* g_ako_config = nullptr;

/* C interface */
int ako_config_init(const std::string& path,
    std::map<std::string, std::string>* config_map) {
    if (!g_ako_config) {
        try {
            g_ako_config = new ako::Config(path, config_map);
        }
        catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            /* error on init config */
            return -1;
        }
    }
    /* successful */
    return 0;
}

void ako_config_exit() {
    if (g_ako_config)
        delete g_ako_config;
    g_ako_config = nullptr;
}

#define AKO_CONFIG_SET_TYPE(alias, type) \
int ako_config_set_##alias (const std::string& section, \
    const std::string& name, const type value) { \
    if (unlikely(g_ako_config == nullptr))  \
        return -1;  \
    g_ako_config->Set(section, name, value);    \
    return 0;   \
}

AKO_CONFIG_SET_TYPE(int, int);
AKO_CONFIG_SET_TYPE(long, long);
AKO_CONFIG_SET_TYPE(double, double);
AKO_CONFIG_SET_TYPE(bool, bool);
AKO_CONFIG_SET_TYPE(string, std::string&);

#define AKO_CONFIG_GET_TYPE(alias, type_flag, default_value, ret_type) \
ret_type ako_config_get_##alias (const std::string& section, \
    const std::string& name) { \
    return (unlikely(g_ako_config == nullptr)) ? \
        default_value :     \
        g_ako_config->Get(section, name, type_flag);    \
}

AKO_CONFIG_GET_TYPE(int, AKO_CONFIG_TYPE_INT, 0, int);
AKO_CONFIG_GET_TYPE(long, AKO_CONFIG_TYPE_INT, 0, long);
AKO_CONFIG_GET_TYPE(double, AKO_CONFIG_TYPE_DOUBLE, 0.0, double);
AKO_CONFIG_GET_TYPE(bool, AKO_CONFIG_TYPE_BOOL, false, bool);
AKO_CONFIG_GET_TYPE(string, AKO_CONFIG_TYPE_STRING, "", std::string);

#define AKO_CONFIG_SET(type) \
int ako_config_set (const std::string& section, \
    const std::string& name, const type value) { \
    if (unlikely(g_ako_config == nullptr))  \
        return -1;  \
    g_ako_config->Set(section, name, value);    \
    return 0;   \
}

AKO_CONFIG_SET(int);
AKO_CONFIG_SET(long);
AKO_CONFIG_SET(double);
AKO_CONFIG_SET(bool);
AKO_CONFIG_SET(char*);
AKO_CONFIG_SET(std::string&);

#define AKO_CONFIG_GET(type_t, type_flag, default_value, ret_type) \
ret_type ako_config_get (const std::string& section, \
    const std::string& name, const type_t) { \
    return (unlikely(g_ako_config == nullptr)) ? \
        default_value :     \
        g_ako_config->Get(section, name, type_flag);    \
}

AKO_CONFIG_GET(ako_config_int_t, AKO_CONFIG_TYPE_INT, 0, long);
AKO_CONFIG_GET(ako_config_double_t, AKO_CONFIG_TYPE_DOUBLE, 0.0, double);
AKO_CONFIG_GET(ako_config_bool_t, AKO_CONFIG_TYPE_BOOL, false, bool);
AKO_CONFIG_GET(ako_config_string_t, AKO_CONFIG_TYPE_STRING, "", std::string);
