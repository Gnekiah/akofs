set(DAEMON_SRCS
    ako_eventd.c
    ako_socket_client.c
    ako_socket_server.c
    ako_timer.cpp
    ako_daemon.cpp)

add_library(daemon_objs STATIC ${DAEMON_SRCS})
target_include_directories(daemon_objs PRIVATE ../include/)

