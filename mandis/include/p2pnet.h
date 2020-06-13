#ifndef MANDIS_MANDIS_P2P_NET_H_
#define MANDIS_MANDIS_P2P_NET_H_

#include <map>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>

#include "../include/config.h"
#include "../include/ssllib.h"
#include "../include/hashlib.h"
#include "../include/logger.h"
#include "../mandis-p2p/kadbucket.h"
#include "../mandis-p2p/req_session.h"

namespace p2pnet {
    
    typedef boost::shared_ptr<ReqSession> req_session_ptr;

    class P2Pnet {
    public:
        P2Pnet(config::Config *config, logger::Logger *logger, boost::asio::io_context& ioc);
        ~P2Pnet();

        void Run();
        void Start();
        void Stop();
        void Join();

    public:
        /// store|<key>|<buffer_size>
        /// access|<key>
        /// sync|<msg>
        /// find|<key>
        /// ping
        int ReqStore(std::string key, std::string block_path);
        int ReqAccess(std::string key);
        int ReqSync(std::string msg);
        int ReqFind(std::string key);
        int Ping(); ///not used

        std::string FindNode(std::string key);

    private:
        ssllib::RsaPair key_;
        hashlib::DigestField id_;
        std::map<int, KadBucket*> * bucket_ = nullptr;
        logger::Logger *logger_ = nullptr;
        config::Config *config_ = nullptr;

    private:
        boost::asio::io_context& ioc_;
    };
}

#endif // !MANDIS_MANDIS_P2P_NET_H_

