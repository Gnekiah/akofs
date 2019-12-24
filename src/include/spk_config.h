/*
 * Configuration interface.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_SPK_CONFIG_H_
#define SPARKLE_SPK_CONFIG_H_

#include <string>
#include <map>

#ifndef SPK_CONFIG_TYPE_T
#define SPK_CONFIG_TYPE_T

typedef int* spk_config_int_t;
typedef char spk_config_bool_t;
typedef double spk_config_double_t;
typedef std::string spk_config_string_t;

#define SPK_CONFIG_TYPE_INT     nullptr
#define SPK_CONFIG_TYPE_BOOL    '0'
#define SPK_CONFIG_TYPE_DOUBLE  1.1
#define SPK_CONFIG_TYPE_STRING  ""

#endif

 /*
  * init config settings from specificated .conf path
  * path: .conf file path
  * config_map: default config mapping
  */
extern int spk_config_init(const std::string& path, 
    std::map<std::string, std::string>* config_map);

extern void spk_config_exit();

extern int spk_config_set_int(const std::string& section, 
    const std::string& name, const int value);
extern int spk_config_set_long(const std::string& section,
    const std::string& name, const long value);
extern int spk_config_set_double(const std::string& section, 
    const std::string& name, const double value);
extern int spk_config_set_bool(const std::string& section, 
    const std::string& name, const bool value);
extern int spk_config_set_string(const std::string& section, 
    const std::string& name, const std::string& value);

extern int spk_config_get_int(const std::string& section, 
    const std::string& name);
extern long spk_config_get_long(const std::string& section, 
    const std::string& name);
extern double spk_config_get_double(const std::string& section,
    const std::string& name);
extern bool spk_config_get_bool(const std::string& section, 
    const std::string& name);
extern std::string spk_config_get_string(const std::string& section,
    const std::string& name);

extern int spk_config_set(const std::string& section,
    const std::string& name, const int value);
extern int spk_config_set(const std::string& section,
    const std::string& name, const long value);
extern int spk_config_set(const std::string& section,
    const std::string& name, const double value);
extern int spk_config_set(const std::string& section,
    const std::string& name, const bool value);
extern int spk_config_set(const std::string& section,
    const std::string& name, const char* value);
extern int spk_config_set(const std::string& section,
    const std::string& name, const std::string& value);

extern long spk_config_get(const std::string& section, 
    const std::string& name, const spk_config_int_t);
extern double spk_config_get(const std::string& section, 
    const std::string& name, const spk_config_double_t);
extern bool spk_config_get(const std::string& section,
    const std::string& name, const spk_config_bool_t);
extern std::string spk_config_get(const std::string& section,
    const std::string& name, const spk_config_string_t);

#endif // SPARKLE_SPK_CONFIG_H_