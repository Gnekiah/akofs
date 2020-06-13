#ifndef MANDIS_MANDIS_P2PNET_SESSION_H_
#define MANDIS_MANDIS_P2PNET_SESSION_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "../include/logger.h"

namespace p2pnet {
    
    class P2PnetSession : public boost::enable_shared_from_this<P2PnetSession> {

    public:
        P2PnetSession(boost::asio::io_context& ioc, logger::Logger* logger);
        ~P2PnetSession();

        void Start();

        boost::asio::ip::tcp::socket& socket();

    private:
        /* Handle completion of a read operation. */
        void HandleRead(const boost::system::error_code &ec, std::size_t bytes_transferred);
        /* Handle completion of a write operation. */
        void HandleWrite(const boost::system::error_code &ec);
        /* Close the session. */
        void Close();

        int Split(std::vector<std::string>& vec, const std::string& str, const char pattern);

    private:
        boost::asio::ip::tcp::socket socket_;
        size_t buffer_length_;
        boost::array<char, 1024 * 512> buffer_;
        logger::Logger* logger_ = nullptr;
    };
}

#endif // !MANDIS_MANDIS_P2PNET_SESSION_H_
