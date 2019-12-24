set(CONFIG_SRCS
    inih/ini.c
    inih/INIReader.cpp
    Config.cpp
    spk_config.cpp)

add_library(config_objs STATIC ${CONFIG_SRCS})
target_include_directories(config_objs PRIVATE ../include/)
