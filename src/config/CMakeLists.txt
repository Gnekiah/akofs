set_property(GLOBAL	PROPERTY USE_FOLDERS ON)

add_definitions(-DLIBCONFIG_STATIC)
add_definitions(-DLIBCONFIGXX_STATIC)

include(GNUInstallDirs)
add_subdirectory(libconfig)

set(CONFIG_SRCS
    a.cc)

add_library(config_objs STATIC ${CONFIG_SRCS})
target_include_directories(config_objs PRIVATE ../include/)
