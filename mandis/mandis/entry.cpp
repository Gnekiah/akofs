#include "../include/entry.h"

namespace frontend {

    Entry::Entry(config::Config* config, foofs::FooFS* foofs, logger::Logger* logger,
        p2pnet::P2Pnet* p2pnet, boost::asio::io_context& ioc)
        : config_(config),
        logger_(logger),
        p2pnet_(p2pnet),
        foofs_(foofs),
        ioc_(ioc),
        acceptor_(ioc_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), config->entry_port()))
    {
        StartAccept();
    }

    Entry::~Entry() {

    }

    void Entry::Run() {

    }

    void Entry::Start() {

    }

    void Entry::Stop() {

    }

    void Entry::Join() {

    }

    void Entry::StartAccept() {
        LOG_TRACE(logger_, "Start Accept");
        session_ptr session(new EntrySession(ioc_, config_, logger_, foofs_, p2pnet_));
        acceptor_.async_accept(session->socket(), boost::bind(&Entry::HandleAccept, this, 
            session, boost::asio::placeholders::error));
    }

    void Entry::HandleAccept(session_ptr session, const boost::system::error_code& ec) {
        LOG_TRACE(logger_, "Handle Accept");
        if (ec)
            LOG_WARNING(logger_, "ecode= " + boost::lexical_cast<std::string>(ec.value()) + ", msg= " + ec.message());
        else
            session->Start();
        StartAccept();
    }
}