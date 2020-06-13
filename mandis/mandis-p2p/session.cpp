#include "session.h"

#include <boost/logic/tribool.hpp>

namespace p2pnet {
    Session::Session(boost::asio::io_service& ios, callback_fn find_fn, callback_fn store_fn,
        callback_fn sync_fn) 
        : socket_(ios),
        rsp_find_callback_(find_fn), 
        rsp_store_callback_(store_fn),
        rsp_sync_callback_(sync_fn)
    {

    }

    Session::~Session() {

    }

    boost::asio::ip::tcp::socket& Session::socket(void) {
        return socket_;
    }

    void Session::Start() {
        socket_.async_read_some(boost::asio::buffer(buffer_), boost::bind(&Session::HandleRead,
            shared_from_this(), boost::asio::placeholders::error, 
            boost::asio::placeholders::bytes_transferred));
    }

    void Session::HandleRead(const boost::system::error_code &ec, std::size_t bytes_transferred) {
        if (ec) {
            LOG_WARNING(logger_, ec.message());
            return;
        }
        int ret = request_parser_.Parse(request_, buffer_.data(), bytes_transferred);
        if (ret == REQUEST_PARSE_SUCCESSFUL) {
            ///TODO: handle request and generate reply info
            //boost::asio::async_write(socket_, )
            ///TODO: return response package, and handle write

        }
        else if (ret == REQUEST_PARSE_DATA_INVALID) {
            ///TODO: generate reply info and return response package, and handle write
        }
        else {
            socket_.async_read_some(boost::asio::buffer(buffer_), boost::bind(&Session::HandleRead,
                shared_from_this(), boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }
    }

    void Session::HandleWrite(const boost::system::error_code &ec) {
        if (ec) {
            LOG_WARNING(logger_, ec.message());
            return;
        }
        ///TODO: handle write
    }

    void Session::Close() {
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }
}
