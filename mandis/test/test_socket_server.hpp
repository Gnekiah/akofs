#ifndef MANDIS_TEST_SOCKET_SERVER_HPP_
#define MANDIS_TEST_SOCKET_SERVER_HPP_

#include "../include/logger.h"
#include "../include/timelib.h"
#include "../mandis-p2p/server.h"
#include "../mandis-p2p/connector.h"

const int TestSocketServerCase1(logger::Logger *log) {
    p2pnet::Server server(log, 12345);
    server.Start();

    ///TODO: connect to server and send message
    boost::asio::io_context ioc;
    p2pnet::Connector connector(ioc, "127.0.0.1", 12345, log);

    timelib::Time::MilliSleep(10000);

    server.Stop();
    server.Join();
    return 0;
}

#endif // !MANDIS_TEST_SOCKET_SERVER_HPP_

