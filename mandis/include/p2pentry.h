#ifndef MANDIS_MANDIS_P2PENTRY_H_
#define MANDIS_MANDIS_P2PENTRY_H_

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include "../mandis/p2pentry_session.h"
#include "../include/logger.h"
#include "../include/config.h"
#include "../include/foofs.h"
#include "../include/p2pnet.h"

namespace frontend {
    typedef boost::shared_ptr<P2PEntrySession> p2pentry_session_ptr;

    class P2PEntry {
    public:
        P2PEntry(config::Config* config, foofs::FooFS* foofs, logger::Logger* logger,
            p2pnet::P2Pnet* p2pnet, boost::asio::io_context& ioc);
        ~P2PEntry();

        void Run();
        void Start();
        void Stop();
        void Join();

    private:
        config::Config* config_ = nullptr;
        foofs::FooFS* foofs_ = nullptr;
        p2pnet::P2Pnet* p2pnet_ = nullptr;
        logger::Logger* logger_ = nullptr;

    private:
        void StartAccept();
        void HandleAccept(p2pentry_session_ptr session, const boost::system::error_code& ec);

    private:
        boost::asio::io_context& ioc_;
        boost::asio::ip::tcp::acceptor acceptor_;
    };
}

#endif // !MANDIS_MANDIS_P2PENTRY_H_
