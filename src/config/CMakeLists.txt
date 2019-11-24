set(CONFIG_SRCS
    inih/ini.c
    inih/INIReader.cpp
    config.cc)

add_library(config_objs STATIC ${CONFIG_SRCS})
target_include_directories(config_objs PRIVATE ../include/)
