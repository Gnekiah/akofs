#include "../include/p2pentry.h"

namespace frontend {
    P2PEntry::P2PEntry(config::Config* config, foofs::FooFS* foofs, logger::Logger* logger,
        p2pnet::P2Pnet* p2pnet, boost::asio::io_context& ioc)
        : config_(config),
        logger_(logger),
        p2pnet_(p2pnet),
        foofs_(foofs),
        ioc_(ioc),
        acceptor_(ioc_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), config->p2pnet_port()))
    {
        StartAccept();
    }

    P2PEntry::~P2PEntry() {

    }

    void P2PEntry::Run() {

    }

    void P2PEntry::Start() {

    }

    void P2PEntry::Stop() {

    }

    void P2PEntry::Join() {

    }

    void P2PEntry::StartAccept() {
        LOG_TRACE(logger_, "Start Accept");
        p2pentry_session_ptr session(new P2PEntrySession(ioc_, config_, logger_, foofs_, p2pnet_));
        acceptor_.async_accept(session->socket(), boost::bind(&P2PEntry::HandleAccept, this,
            session, boost::asio::placeholders::error));
    }

    void P2PEntry::HandleAccept(p2pentry_session_ptr session, const boost::system::error_code& ec) {
        LOG_TRACE(logger_, "Handle Accept");
        if (ec)
            LOG_WARNING(logger_, "ecode= " + boost::lexical_cast<std::string>(ec.value()) + ", msg= " + ec.message());
        else
            session->Start();
        StartAccept();
    }
}