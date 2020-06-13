#ifndef MANDIS_MANDIS_P2P_UDP_CLIENT_H_
#define MANDIS_MANDIS_P2P_UDP_CLIENT_H_

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/bind/bind.hpp>
#include "../include/logger.h"

namespace p2pnet {

    class UdpClient {
    public:
        UdpClient(boost::asio::io_context &ioc, std::string &ip, int port, logger::Logger *logger);

    private:
        boost::asio::ip::udp::socket socket_;
        logger::Logger *logger_;
    };




}

#endif // !MANDIS_MANDIS_P2P_UDP_CLIENT_H_

