#include "../include/config.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <boost/filesystem.hpp>
#elif __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif 

using boost::property_tree::ini_parser::read_ini;

namespace config {
    Config::Config(std::string &config_filepath) 
        : config_filepath_(config_filepath)
    {
        std::ifstream fin(config_filepath.c_str());
        if (!fin) {
            std::cerr << "Config File Not Found, Do GenConfig()" << std::endl;
            GenConfig();
            boost::property_tree::ini_parser::write_ini(config_filepath, pt_);
            exit(0);
        }

        boost::property_tree::ini_parser::read_ini(config_filepath, pt_);

        logging_filepath_ = pt_.get<std::string>("Logger.LoggingPath", "mandis.log");
        logging_level_ = pt_.get<int>("Logger.LoggingLevel", 3);

        entry_port_ = pt_.get<unsigned short>("Mandis.Port", 60001);

        block_path_ = pt_.get<std::string>("MandisFS.BlockPath", "block");
        block_filepath_ = pt_.get<std::string>("MandisFS.BlockInfoPath", "");
        file_filepath_ = pt_.get<std::string>("MandisFS.FileInfoPath", "");

        p2pnet_port_ = pt_.get<unsigned short>("MandisP2P.Port", 60002);
        p2pnet_filepath_ = pt_.get<std::string>("MandisP2P.NetInfoPath", "");
        rsa_key_length_ = pt_.get<int>("MandisP2P.RSAKeyLength", 2048);
        rsa_password_ = pt_.get<std::string>("MandisP2P.RSAPassword", "");
        rsa_public_key_path_ = pt_.get<std::string>("MandisP2P.RSAPublicKeyPath", "");
        rsa_private_key_path_ = pt_.get<std::string>("MandisP2P.RSAPrivateKeyPath", "");

#ifdef _WIN32
        if (!boost::filesystem::exists(block_path()) || !boost::filesystem::is_directory(block_path()))
            boost::filesystem::create_directories(block_path());
#elif __linux__
        struct stat st = { 0 };
        if (stat(block_path().c_str(), &st) == -1) {
            mkdir(block_path().c_str(), 0755);
        }
#endif
    }

    Config::~Config() {

    }

    void Config::GenConfig() {
        pt_.put<std::string>("Logger.LoggingPath", "mandis.log");
        pt_.put<std::string>("Logger.LoggingLevel", "3");

        pt_.put<std::string>("Mandis.Port", "60001");

        pt_.put<std::string>("MandisFS.BlockPath", "block");
        pt_.put<std::string>("MandisFS.BlockInfoPath", "");
        pt_.put<std::string>("MandisFS.FileInfoPath", "");

        pt_.put<std::string>("MandisP2P.Port", "60002");
        pt_.put<std::string>("MandisP2P.NetInfoPath", "");
        pt_.put<std::string>("MandisP2P.RSAKeyLength", "");
        pt_.put<std::string>("MandisP2P.RSAPassword", "");
        pt_.put<std::string>("MandisP2P.RSAPublicKeyPath", "");
        pt_.put<std::string>("MandisP2P.RSAPrivateKeyPath", "");
    }

}
