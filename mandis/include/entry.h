#ifndef MANDIS_MANDIS_ENTRY_H_
#define MANDIS_MANDIS_ENTRY_H_

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include "../mandis/entry_session.h"
#include "../include/logger.h"
#include "../include/config.h"
#include "../include/foofs.h"
#include "../include/p2pnet.h"

namespace frontend {

    typedef boost::shared_ptr<EntrySession> session_ptr;

    class Entry {
    public:
        Entry(config::Config *config, foofs::FooFS *foofs, logger::Logger *logger, 
            p2pnet::P2Pnet* p2pnet, boost::asio::io_context& ioc);
        ~Entry();

        void Run();
        void Start();
        void Stop();
        void Join();

    private:
        config::Config *config_ = nullptr;
        foofs::FooFS *foofs_ = nullptr;
        p2pnet::P2Pnet* p2pnet_ = nullptr;
        logger::Logger *logger_ = nullptr;

    private:
        void StartAccept();
        void HandleAccept(session_ptr session, const boost::system::error_code& ec);

    private:
        boost::asio::io_context& ioc_;
        boost::asio::ip::tcp::acceptor acceptor_;
    };
}

#endif // !MANDIS_MANDIS_ENTRY_H_

