#include "req_session.h"
#include <fstream>
#include <boost/lexical_cast.hpp>

namespace p2pnet {

    ReqSession::ReqSession(boost::asio::io_context& ioc, const std::string ip, const int port,
        logger::Logger* logger)
        : ioc_(ioc),
        socket_(ioc),
        logger_(logger),
        ep_(boost::asio::ip::address::from_string(ip), port)
    {
        LOG_INFO(logger_, "Req Session Init");
        socket_.connect(ep_);
        //boost::asio::connect(socket_, ep_);
    }

    int ReqSession::DoReqStore(std::string key, std::string block_path) {
        std::ifstream fin(block_path, std::ios::binary);
        if (!fin) {
            LOG_WARNING(logger_, "Block File Not Found");
            return -1;
        }

        fin.read(buffer_.data(), 1024 * 512);
        buffer_length_ = fin.gcount();
        fin.close();

        /// command: store|<key>|<buffer_size>
        std::string cmd = "store|" + key + "|" + boost::lexical_cast<std::string>(buffer_length_);
        LOG_TRACE(logger_, "Send Command: " + cmd);
        boost::asio::write(socket_, boost::asio::buffer(cmd.data(), cmd.length()));
        LOG_TRACE(logger_, "Waiting For Remote");
        boost::asio::streambuf response;
        boost::asio::read_until(socket_, response, "\n");
        LOG_TRACE(logger_, "Receive Back Message");

        LOG_TRACE(logger_, "Send Data, Size=" + boost::lexical_cast<std::string>(buffer_length_));
        boost::asio::write(socket_, boost::asio::buffer(buffer_, buffer_length_));
        socket_.close();
        return 0;
    }

    int ReqSession::DoReqAccess(std::string key, std::string block_path) {
        /// command: access|<key>
        std::string cmd = "access|" + key;
        LOG_TRACE(logger_, "Send Command: " + cmd);
        boost::asio::write(socket_, boost::asio::buffer(cmd.data(), cmd.length()));
        buffer_length_ = socket_.read_some(boost::asio::buffer(buffer_, 1024 * 512));
        LOG_TRACE(logger_, "Receive Data, Size=" + boost::lexical_cast<std::string>(buffer_length_));
        boost::asio::read(socket_, boost::asio::buffer(buffer_, buffer_length_));

        std::ofstream fout(block_path, std::ios::binary);
        if (!fout) {
            LOG_WARNING(logger_, "Block File Cannot Open");
            return -1;
        }
        fout.write(buffer_.data(), buffer_length_);
        fout.close();
        socket_.close();
        return 0;
    }

    int ReqSession::DoReqSync(std::string msg) {
        /// command: sync|<msg>
        std::string cmd = "sync|" + msg;
        LOG_TRACE(logger_, "Send Command: " + cmd);
        boost::asio::write(socket_, boost::asio::buffer(cmd.data(), cmd.length()));
        socket_.close();
        return 0;
    }

    std::string ReqSession::DoReqFind(std::string key) {
        /// command: sync|<msg>
        std::string cmd = "sync|" + key;
        LOG_TRACE(logger_, "Send Command: " + cmd);
        boost::asio::write(socket_, boost::asio::buffer(cmd.data(), cmd.length()));
        buffer_length_ = socket_.read_some(boost::asio::buffer(buffer_, 1024 * 512));

        LOG_TRACE(logger_, "Receive Data, Size=" + boost::lexical_cast<std::string>(buffer_length_));
        boost::asio::read(socket_, boost::asio::buffer(buffer_, buffer_length_));
        socket_.close();
        ///TODO
        buffer_.data()[buffer_length_] = '\0';
        std::string found_buffer = std::string(buffer_.data());
        LOG_TRACE(logger_, "On Response ReqFind: " + found_buffer);
        return found_buffer;
    }

    int ReqSession::DoReqPing() {
        return 0;
    }
}