#ifndef MANDIS_MANDIS_P2P_CONNECTOR_H_
#define MANDIS_MANDIS_P2P_CONNECTOR_H_

#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../include/logger.h"


namespace p2pnet {
    class Connector {
    public:
        Connector(boost::asio::io_context& ioc, const std::string &ip, int port, logger::Logger *log);
        ~Connector();

        void ReqPing();
        void ReqFind();
        void ReqStore();
        void ReqSync();

        void StartConnector();
        /* Initiate an asynchronous accept operation. */
        void Start();
        void Join();
        /* Stop the server. */
        void Stop();

        void HandleConnect(const boost::system::error_code& ec);

    private:
        boost::asio::io_context ioc_;
        boost::asio::ip::tcp::socket socket_;
        boost::asio::streambuf request_buff_;
        boost::asio::streambuf response_buff_;
        logger::Logger * logger_ = nullptr;
    };
}

#endif // !MANDIS_MANDIS_P2P_CONNECTOR_H_
