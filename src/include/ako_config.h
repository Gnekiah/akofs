/*
 * Configuration interface.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_AKO_CONFIG_H_
#define AKOFS_AKO_CONFIG_H_

#include <string>
#include <map>

#ifndef AKO_CONFIG_TYPE_T
#define AKO_CONFIG_TYPE_T

typedef int* ako_config_int_t;
typedef char ako_config_bool_t;
typedef double ako_config_double_t;
typedef std::string ako_config_string_t;

#define AKO_CONFIG_TYPE_INT     nullptr
#define AKO_CONFIG_TYPE_BOOL    '0'
#define AKO_CONFIG_TYPE_DOUBLE  1.1
#define AKO_CONFIG_TYPE_STRING  ""

#endif

 /*
  * init config settings from specificated .conf path
  * path: .conf file path
  * config_map: default config mapping
  */
extern int ako_config_init(const std::string& path, 
    std::map<std::string, std::string>* config_map);

extern void ako_config_exit();

extern int ako_config_set_int(const std::string& section, 
    const std::string& name, const int value);
extern int ako_config_set_long(const std::string& section,
    const std::string& name, const long value);
extern int ako_config_set_double(const std::string& section, 
    const std::string& name, const double value);
extern int ako_config_set_bool(const std::string& section, 
    const std::string& name, const bool value);
extern int ako_config_set_string(const std::string& section, 
    const std::string& name, const std::string& value);

extern int ako_config_get_int(const std::string& section, 
    const std::string& name);
extern long ako_config_get_long(const std::string& section, 
    const std::string& name);
extern double ako_config_get_double(const std::string& section,
    const std::string& name);
extern bool ako_config_get_bool(const std::string& section, 
    const std::string& name);
extern std::string ako_config_get_string(const std::string& section,
    const std::string& name);

extern int ako_config_set(const std::string& section,
    const std::string& name, const int value);
extern int ako_config_set(const std::string& section,
    const std::string& name, const long value);
extern int ako_config_set(const std::string& section,
    const std::string& name, const double value);
extern int ako_config_set(const std::string& section,
    const std::string& name, const bool value);
extern int ako_config_set(const std::string& section,
    const std::string& name, const char* value);
extern int ako_config_set(const std::string& section,
    const std::string& name, const std::string& value);

extern long ako_config_get(const std::string& section, 
    const std::string& name, const ako_config_int_t);
extern double ako_config_get(const std::string& section, 
    const std::string& name, const ako_config_double_t);
extern bool ako_config_get(const std::string& section,
    const std::string& name, const ako_config_bool_t);
extern std::string ako_config_get(const std::string& section,
    const std::string& name, const ako_config_string_t);

#endif // AKOFS_AKO_CONFIG_H_