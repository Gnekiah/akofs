#include "p2pentry_session.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

namespace frontend {
    P2PEntrySession::P2PEntrySession(boost::asio::io_context& ioc, config::Config* config,
        logger::Logger* logger, foofs::FooFS* foofs, p2pnet::P2Pnet* p2pnet)
        : socket_(ioc),
        config_(config),
        logger_(logger),
        foofs_(foofs),
        p2pnet_(p2pnet)
    {

    }

    P2PEntrySession::~P2PEntrySession() {

    }

    void P2PEntrySession::Start() {
        LOG_TRACE(logger_, "Start Session");
        socket_.async_read_some(boost::asio::buffer(buffer_.data(), 1024 * 512),
            boost::bind(&P2PEntrySession::HandleReadForDispatch, shared_from_this(), boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    boost::asio::ip::tcp::socket& P2PEntrySession::socket() {
        return socket_;
    }

    /// store|<key>|<buffer_size>
    /// access|<key>
    /// sync|<msg>
    /// find|<key>
    /// ping
    void P2PEntrySession::HandleReadForDispatch(const boost::system::error_code& ec, std::size_t bytes_transferred) {
        LOG_TRACE(logger_, "Handle For Dispatch");
        if (ec) {
            LOG_WARNING(logger_, "ecode= " + boost::lexical_cast<std::string>(ec.value()) + ", msg= " + ec.message());
            return;
        }

        int ret = 0;
        std::vector<std::string> vec;
        buffer_.data()[bytes_transferred] = '\0';
        std::string cmdstr = buffer_.data();
        LOG_TRACE(logger_, "Command=" + cmdstr);

        ret = foofs_->Split(vec, cmdstr, '|');
        std::string cmd_ops = vec[0];

        /// store|<key>|<buffer_size>
        if (cmd_ops == "store" && ret == 3) {
            LOG_TRACE(logger_, "Rsp Store");
            buffer_length_ = boost::lexical_cast<int>(vec[2]);
            boost::asio::write(socket_, boost::asio::buffer("ready for receiving\n"));

            std::string key = vec[1];
            boost::asio::read(socket_, boost::asio::buffer(buffer_, buffer_length_));

            boost::filesystem::path out_path = boost::filesystem::path(config_->block_path()) / key;
            std::ofstream fout(out_path.string(), std::ios::binary);
            fout.write(buffer_.data(), buffer_length_);
            fout.close();
        }
        /// access|<key>
        else if (cmd_ops == "access" && ret == 2) {
            LOG_TRACE(logger_, "Rsp Access");
        }
        /// sync|<msg>
        else if (cmd_ops == "sync" && ret == 2) {
            LOG_TRACE(logger_, "Rsp Sync");
        }
        /// find|<key>
        else if (cmd_ops == "find" && ret == 2) {
            LOG_TRACE(logger_, "Rsp Find");
        }
        /// ping
        else if (cmd_ops == "ping") {
            LOG_TRACE(logger_, "Rsp Ping");
        }
        else {
            LOG_WARNING(logger_, "Command Error, cmd=" + cmdstr);
            Close();
        }
    }

    void P2PEntrySession::HandleCompleted(const boost::system::error_code& ec) {
        if (ec) {
            LOG_WARNING(logger_, "ecode= " + boost::lexical_cast<std::string>(ec.value()) + ", msg= " + ec.message());
            return;
        }
        //Close();
    }

    void P2PEntrySession::Close() {
        boost::system::error_code ignored_ec;
        try {
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        }
        catch (std::exception& e) {
            LOG_TRACE(logger_, e.what());
        }
    }
}