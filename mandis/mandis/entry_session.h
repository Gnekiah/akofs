#ifndef MANDIS_MANDIS_ENTRY_SESSION_H_
#define MANDIS_MANDIS_ENTRY_SESSION_H_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "../include/logger.h"
#include "../include/foofs.h"
#include "../include/p2pnet.h"
#include "../include/config.h"

namespace frontend {

    class EntrySession : public boost::enable_shared_from_this<EntrySession> {
    public:
        EntrySession(boost::asio::io_context& ioc, config::Config* config,
            logger::Logger *logger, foofs::FooFS *foofs, p2pnet::P2Pnet* p2pnet);
        ~EntrySession();

        void Start();

        boost::asio::ip::tcp::socket& socket();

    private:
        /* Handle completion of a read operation. */
        void HandleReadForDispatch(const boost::system::error_code &ec, std::size_t bytes_transferred);
        /* Handle completion of a write operation. */
        void HandleCompleted(const boost::system::error_code &ec);
        /* Close the session. */
        void Close();

    private:
        size_t buffer_length_;
        boost::array<char, 1024 * 512> buffer_;
        boost::asio::ip::tcp::socket socket_;
        config::Config* config_ = nullptr;
        logger::Logger *logger_ = nullptr;
        foofs::FooFS *foofs_ = nullptr;
        p2pnet::P2Pnet* p2pnet_ = nullptr;
    };
}

#endif // !MANDIS_MANDIS_ENTRY_SESSION_H_

