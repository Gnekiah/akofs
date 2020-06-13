#include "../include/p2pnet.h"
#include <boost/filesystem.hpp>

namespace p2pnet {
    P2Pnet::P2Pnet(config::Config *config, logger::Logger *logger, boost::asio::io_context& ioc)
        : key_(config->rsa_private_key_path(), config->rsa_public_key_path(), config->rsa_password(), logger),
        config_(config),
        logger_(logger),
        ioc_(ioc)
    {

    }

    P2Pnet::~P2Pnet() {

    }

    void P2Pnet::Run() {

    }

    void P2Pnet::Start() {

    }

    void P2Pnet::Stop() {

    }

    void P2Pnet::Join() {

    }

    int P2Pnet::ReqStore(std::string key, std::string block_path) {
        ReqSession session(ioc_, "127.0.0.1", 60002, logger_);
        session.DoReqStore(key, block_path);
        return 0;
    }

    int P2Pnet::ReqAccess(std::string key) {
        ReqSession session(ioc_, "127.0.0.1", 60002, logger_);
        boost::filesystem::path out_path = boost::filesystem::path(config_->block_path()) / key;
        session.DoReqAccess(key, out_path.string());
        return 0;

    }

    int P2Pnet::ReqSync(std::string msg) {
        ReqSession session(ioc_, "127.0.0.1", 60002, logger_);
        session.DoReqSync(msg);
        return 0;

    }

    int P2Pnet::ReqFind(std::string key) {
        ReqSession session(ioc_, "127.0.0.1", 60002, logger_);
        std::string retmsg = session.DoReqFind(key);
        //
        return 0;

    }

    int P2Pnet::Ping() {
        ReqSession session(ioc_, "127.0.0.1", 60002, logger_);
        session.DoReqPing();
        return 0;
    }

    std::string P2Pnet::FindNode(std::string key) {
        std::string ret_str;

        return ret_str;
    }
}