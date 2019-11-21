
set(ARCH_SRCS
    spk_setproctitle.c)

add_library(arch_objs STATIC ${ARCH_SRCS})
target_include_directories(arch_objs PRIVATE ../include/)
