/*
 * Configuration interface.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#include <Config.hpp>
#include <spk_config.h>
#include <spk_compat.h>

static spk::Config* g_spk_config = nullptr;

/* C interface */
int spk_config_init(const std::string& path,
    std::map<std::string, std::string>* config_map) {
    if (!g_spk_config) {
        try {
            g_spk_config = new spk::Config(path, config_map);
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

void spk_config_exit() {
    if (g_spk_config)
        delete g_spk_config;
    g_spk_config = nullptr;
}

#define SPK_CONFIG_SET_TYPE(alias, type) \
int spk_config_set_##alias (const std::string& section, \
    const std::string& name, const type value) { \
    if (unlikely(g_spk_config == nullptr))  \
        return -1;  \
    g_spk_config->Set(section, name, value);    \
    return 0;   \
}

SPK_CONFIG_SET_TYPE(int, int);
SPK_CONFIG_SET_TYPE(long, long);
SPK_CONFIG_SET_TYPE(double, double);
SPK_CONFIG_SET_TYPE(bool, bool);
SPK_CONFIG_SET_TYPE(string, std::string&);

#define SPK_CONFIG_GET_TYPE(alias, type_flag, default_value, ret_type) \
ret_type spk_config_get_##alias (const std::string& section, \
    const std::string& name) { \
    return (unlikely(g_spk_config == nullptr)) ? \
        default_value :     \
        g_spk_config->Get(section, name, type_flag);    \
}

SPK_CONFIG_GET_TYPE(int, SPK_CONFIG_TYPE_INT, 0, int);
SPK_CONFIG_GET_TYPE(long, SPK_CONFIG_TYPE_INT, 0, long);
SPK_CONFIG_GET_TYPE(double, SPK_CONFIG_TYPE_DOUBLE, 0.0, double);
SPK_CONFIG_GET_TYPE(bool, SPK_CONFIG_TYPE_BOOL, false, bool);
SPK_CONFIG_GET_TYPE(string, SPK_CONFIG_TYPE_STRING, "", std::string);

#define SPK_CONFIG_SET(type) \
int spk_config_set (const std::string& section, \
    const std::string& name, const type value) { \
    if (unlikely(g_spk_config == nullptr))  \
        return -1;  \
    g_spk_config->Set(section, name, value);    \
    return 0;   \
}

SPK_CONFIG_SET(int);
SPK_CONFIG_SET(long);
SPK_CONFIG_SET(double);
SPK_CONFIG_SET(bool);
SPK_CONFIG_SET(char*);
SPK_CONFIG_SET(std::string&);

#define SPK_CONFIG_GET(type_t, type_flag, default_value, ret_type) \
ret_type spk_config_get (const std::string& section, \
    const std::string& name, const type_t) { \
    return (unlikely(g_spk_config == nullptr)) ? \
        default_value :     \
        g_spk_config->Get(section, name, type_flag);    \
}

SPK_CONFIG_GET(spk_config_int_t, SPK_CONFIG_TYPE_INT, 0, long);
SPK_CONFIG_GET(spk_config_double_t, SPK_CONFIG_TYPE_DOUBLE, 0.0, double);
SPK_CONFIG_GET(spk_config_bool_t, SPK_CONFIG_TYPE_BOOL, false, bool);
SPK_CONFIG_GET(spk_config_string_t, SPK_CONFIG_TYPE_STRING, "", std::string);
