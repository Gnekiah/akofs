set(CORE_SRCS
    ako_init.cpp)

add_library(core_objs STATIC ${CORE_SRCS})
target_include_directories(core_objs PRIVATE ../include/)
