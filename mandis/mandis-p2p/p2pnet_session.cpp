#include "p2pnet_session.h"
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

namespace p2pnet {
    P2PnetSession::P2PnetSession(boost::asio::io_context& ioc, logger::Logger* logger)
        : socket_(ioc),
        logger_(logger)
    {

    }

    P2PnetSession::~P2PnetSession() {

    }

    void P2PnetSession::Start() {
        LOG_TRACE(logger_, "Start Session");
        socket_.async_read_some(boost::asio::buffer(buffer_.data(), 1024 * 512),
            boost::bind(&P2PnetSession::HandleRead, shared_from_this(), boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    boost::asio::ip::tcp::socket& P2PnetSession::socket() {
        return socket_;
    }

    void P2PnetSession::HandleRead(const boost::system::error_code &ec, std::size_t bytes_transferred) {
        LOG_TRACE(logger_, "Handle Read");
        if (ec) {
            LOG_WARNING(logger_, "ecode= " + boost::lexical_cast<std::string>(ec.value()) + ", msg= " + ec.message());
            return;
        }

        int ret = 0;
        std::vector<std::string> vec;
        std::string cmdstr = buffer_.data();
        ret = Split(vec, cmdstr, '|');

        /// store|<key>|<buffer_size>
        /// access|<key>
        /// sync|<msg>
        /// find|<key>
        /// ping
        if (ret == 3 && vec[0] == "store") {
            LOG_TRACE(logger_, "Rsp Store");
            buffer_length_ = boost::lexical_cast<int>(vec[2]);
            std::string key = vec[1];
            boost::asio::read(socket_, boost::asio::buffer(buffer_, buffer_length_));

            //boost::filesystem::path out_path = boost::filesystem::path(config_->block_path()) / key / "--";
            //std::ofstream fout(out_path.string(), std::ios::binary);
            //fout.write(buffer_.data(), buffer_length_);
            //fout.close();
            ///TODO!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        else if (ret == 2 && vec[0] == "access") {
            LOG_TRACE(logger_, "Rsp Access");

        }
        else if (ret == 2 && vec[0] == "sync") {
            LOG_TRACE(logger_, "Rsp Sync");

        }
        else if (ret == 2 && vec[0] == "find") {
            LOG_TRACE(logger_, "Rsp Find");

        }
        else if (ret == 1 && vec[0] == "ping") {
            // not used
        }
        else {
            LOG_WARNING(logger_, "Command Error, cmd=" + cmdstr);
            Close();
        }
    }

    void P2PnetSession::HandleWrite(const boost::system::error_code &ec) {
        
    }

    void P2PnetSession::Close() {
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }

    int P2PnetSession::Split(std::vector<std::string>& vec, const std::string& str, const char pattern)
    {
        std::stringstream input(str);
        std::string temp;
        while (getline(input, temp, pattern))
        {
            vec.push_back(temp);
        }
        return (int)vec.size();
    }
}