#ifndef MANDIS_MANDIS_P2P_SESSION_H_
#define MANDIS_MANDIS_P2P_SESSION_H_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "request_parser.h"
#include "request.hpp"
#include "request_handler.h"
#include "../include/logger.h"

#define BUFFER_SIZE (256 * 1024 + 64)

namespace p2pnet {
    typedef boost::function<void(int, int)> callback_fn;

    /* Represents a single connection from a client. */
    class Session : public boost::enable_shared_from_this<Session> {
    public:
        Session(boost::asio::io_service& ios, callback_fn find_fn, callback_fn store_fn, 
            callback_fn sync_fn);
        virtual ~Session();

        /* Start the first asynchronous operation for the connection. */
        void Start();
        /* Get the socket associated with the connection. */
        boost::asio::ip::tcp::socket& socket(void);

    private:
        /* Handle completion of a read operation. */
        void HandleRead(const boost::system::error_code &ec, std::size_t bytes_transferred);
        /* Handle completion of a write operation. */
        void HandleWrite(const boost::system::error_code &ec);
        /* Close the session. */
        void Close();

    private:
        /* Buffer size for incoming data. */
        int buffer_size_ = BUFFER_SIZE;
        /* Buffer for incoming data. */
        boost::array<char, BUFFER_SIZE> buffer_;
        /* Socket for the connection. */
        boost::asio::ip::tcp::socket socket_;
        /* The incoming request. */
        Request request_;
        /* The handler used to process the incoming request. */
        RequestParser request_parser_;
        /* The handler used to process the incoming request. */
        RequestHandler request_handler_;

        callback_fn rsp_find_callback_;
        callback_fn rsp_store_callback_;
        callback_fn rsp_sync_callback_;
        logger::Logger *logger_ = nullptr;
    };

}

#endif // !MANDIS_MANDIS_P2P_SESSION_H_
