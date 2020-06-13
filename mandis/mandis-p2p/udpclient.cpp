#include "udpclient.h"


namespace p2pnet {

    UdpClient::UdpClient(boost::asio::io_context &ioc, std::string &ip, int port,
        logger::Logger *logger)
        : socket_(ioc),
        logger_(logger)
    {
        boost::asio::ip::udp::endpoint ep(boost::asio::ip::address::from_string(ip), port);
        socket_.async_connect(ep, boost::bind(&Connector::HandleConnect, this,
            boost::asio::placeholders::error));
    }

}