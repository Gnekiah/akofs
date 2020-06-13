#include "entry_session.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

namespace frontend {

    EntrySession::EntrySession(boost::asio::io_context& ioc, config::Config* config, 
        logger::Logger *logger, foofs::FooFS *foofs, p2pnet::P2Pnet* p2pnet)
        : socket_(ioc),
        config_(config),
        logger_(logger),
        foofs_(foofs),
        p2pnet_(p2pnet)
    {

    }

    EntrySession::~EntrySession() {

    }

    void EntrySession::Start() {
        LOG_TRACE(logger_, "Start Session");
        socket_.async_read_some(boost::asio::buffer(buffer_.data(), 1024 * 512),
            boost::bind(&EntrySession::HandleReadForDispatch, shared_from_this(), boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    boost::asio::ip::tcp::socket& EntrySession::socket() {
        return socket_;
    }

    /// list|<>|<>
    /// read|<filename>|<filepath>
    /// write|<filepath>|<>
    /// store|<key>|<buffer_size>
    /// access|<key>
    /// sync|<msg>
    /// find|<key>
    /// ping
    void EntrySession::HandleReadForDispatch(const boost::system::error_code &ec, std::size_t bytes_transferred) {
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

        /// list|<>|<>
        if (cmd_ops == "list" && ret == 3) {
            std::string msgback;
            char msg[4096];

            ret = foofs_->ReadMetaData(msg);
            if (ret > 0) {
                msg[ret] = '\0';
                msgback = msg;
            }
            else {
                msgback = "Failed";
            }
            boost::asio::async_write(socket_, boost::asio::buffer(msgback.c_str(), msgback.length()),
                boost::bind(&EntrySession::HandleCompleted, this, boost::asio::placeholders::error));
        }
        /// write|<filepath>|<>
        else if (cmd_ops == "writ" && ret == 3) {
            std::string msgback;
            std::string cmd_filepath = vec[1];

            ret = foofs_->Write(cmd_filepath);
            if (ret == 0) {
                LOG_INFO(logger_, "File Wrote: " + cmd_filepath);
                msgback = "Completed";
            }
            else {
                msgback = "Failed";
            }
            boost::asio::async_write(socket_, boost::asio::buffer(msgback.c_str(), msgback.length()),
                boost::bind(&EntrySession::HandleCompleted, this, boost::asio::placeholders::error));
        }
        /// read|<filename>|<filepath>
        else if (cmd_ops == "read" && ret == 3) {
            std::string msgback;
            std::string cmd_filepath = vec[1];
            std::string cmd_filename = vec[2];

            ret = foofs_->ReadByName(cmd_filename, cmd_filepath);
            if (ret == 0) {
                LOG_INFO(logger_, "File Read: " + cmd_filename);
                msgback = "Completed";
            }
            else {
                msgback = "Failed";
            }
            boost::asio::async_write(socket_, boost::asio::buffer(msgback.c_str(), msgback.length()),
                boost::bind(&EntrySession::HandleCompleted, this, boost::asio::placeholders::error));
        }
        /// store|<key>|<buffer_size>
        else if (cmd_ops == "store" && ret == 3) {
            LOG_TRACE(logger_, "Rsp Store");
            buffer_length_ = boost::lexical_cast<int>(vec[2]);
            boost::asio::write(socket_, boost::asio::buffer("ready for receiving\n"));

            std::string key = vec[1];
            boost::asio::read(socket_, boost::asio::buffer(buffer_, buffer_length_));

            boost::filesystem::path out_path = boost::filesystem::path(config_->block_path()) / key / "--";
            std::ofstream fout(out_path.string(), std::ios::binary);
            fout.write(buffer_.data(), buffer_length_);
            fout.close();
            ///TODO!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        /// access|<key>
        else if (cmd_ops == "access" && ret == 2) {
            LOG_TRACE(logger_, "Rsp Access");
            // not used
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
            // not used
        }
        else {
            LOG_WARNING(logger_, "Command Error, cmd=" + cmdstr);
            Close();
        }
    }

    void EntrySession::HandleCompleted(const boost::system::error_code &ec) {
        if (ec) {
            LOG_WARNING(logger_, "ecode= " + boost::lexical_cast<std::string>(ec.value()) + ", msg= " + ec.message());
            return;
        }
        //Close();
    }

    void EntrySession::Close() {
        boost::system::error_code ignored_ec;
        try {
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        }
        catch (std::exception &e) {
            LOG_TRACE(logger_, e.what());
        }
    }
}
