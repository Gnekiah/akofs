#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "../include/logger.h"
#include "../include/config.h"
#include "../include/p2pentry.h"
#include "../include/entry.h"
#include "../include/foofs.h"
#include "../include/p2pnet.h"

int main(int argc, char **argv)
{
    std::string config_path = "mandis.cfg";
    config::Config *config = new config::Config(config_path);
    logger::Logger *logger = new logger::Logger(config->logging_filepath(), config->logging_level());
    logger->Start();

    LOG_INFO(logger, "");
    LOG_INFO(logger, "Mandis Start!");
    LOG_TRACE(logger, "=============== Configuration Begin ===============");
    LOG_TRACE(logger, "block_filepath= " + config->block_filepath());
    LOG_TRACE(logger, "block_path= " + config->block_path());
    LOG_TRACE(logger, "entry_port= " + boost::lexical_cast<std::string>(config->entry_port()));
    LOG_TRACE(logger, "file_filepath= " + config->file_filepath());
    LOG_TRACE(logger, "logging_filepath= " + config->logging_filepath());
    LOG_TRACE(logger, "logging_level= " + boost::lexical_cast<std::string>(config->logging_level()));
    LOG_TRACE(logger, "p2pnet_filepath= " + config->p2pnet_filepath());
    LOG_TRACE(logger, "p2pnet_port= " + boost::lexical_cast<std::string>(config->p2pnet_port()));
    LOG_TRACE(logger, "rsa_key_length= " + boost::lexical_cast<std::string>(config->rsa_key_length()));
    LOG_TRACE(logger, "rsa_password= " + config->rsa_password());
    LOG_TRACE(logger, "rsa_private_key_path= " + config->rsa_private_key_path());
    LOG_TRACE(logger, "rsa_public_key_path= " + config->rsa_public_key_path());
    LOG_TRACE(logger, "=============== Configuration End ===============");

    boost::asio::io_context ioc;
    boost::asio::io_context ioc2;

    auto f = [&]() { ioc2.run(); };

    LOG_INFO(logger, "P2Pnet Init...");
    p2pnet::P2Pnet *p2pnet = new p2pnet::P2Pnet(config, logger, ioc);
    p2pnet->Start();

    LOG_INFO(logger, "FooFS Init...");
    foofs::FooFS *foofs = new foofs::FooFS(config, p2pnet, logger);
    foofs->Start();

    LOG_INFO(logger, "P2P Entry Init...");
    frontend::P2PEntry* p2pentry = new frontend::P2PEntry(config, foofs, logger, p2pnet, ioc2);
    p2pentry->Start();

    LOG_INFO(logger, "Entry Init...");
    frontend::Entry *entry = new frontend::Entry(config, foofs, logger, p2pnet, ioc);
    entry->Start();

    LOG_INFO(logger, "Mandis running...");
    boost::thread thrd(f);
    ioc.run();
    thrd.join();
    LOG_INFO(logger, "Mandis stopped!");

    entry->Join();
    foofs->Join();
    p2pnet->Join();
    logger->Join();

    delete entry;
    delete foofs;
    delete p2pnet;
    delete logger;

    return 0;
}
 