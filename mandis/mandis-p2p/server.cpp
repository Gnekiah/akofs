#include "server.h"

#include "session.h"

namespace p2pnet {    

    Server::Server(logger::Logger *logger, int port)
        : logger_(logger),
        acceptor_(ios_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        Run();
    }

    Server::~Server() {

    }
    
    void Server::StartServer() {
        boost::system::error_code ec;
        ios_.run(ec);
        LOG_DEBUG(logger_, ec.message());
    }

    void Server::Start() {
        LOG_TRACE(logger_, "Server Start!");
        thrd_ = boost::thread(boost::bind(&Server::StartServer, this));
    }

    void Server::Join() {
        LOG_TRACE(logger_, "Server Join!");
        thrd_.join();
    }

    void Server::Stop() {
        ios_.stop();
        LOG_TRACE(logger_, "Server Stop!");
        thrd_.interrupt();
    }

    void Server::Run() {
        session_ptr session(new Session(ios_, rsp_find_callback_, rsp_store_callback_,
            rsp_sync_callback_));
        acceptor_.async_accept(session->socket(), boost::bind(&Server::HandleAccept, 
            this, session, boost::asio::placeholders::error));
    }

    
    void Server::HandleAccept(session_ptr session, const boost::system::error_code& ec) {
        if (ec || !session) {
            LOG_WARNING(logger_, ec.message());
            return;
        }
        session->Start();
        Run();
    }
}