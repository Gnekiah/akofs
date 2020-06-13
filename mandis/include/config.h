#ifndef MANDIS_MANDIS_CONFIG_H_
#define MANDIS_MANDIS_CONFIG_H_

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace config {

    typedef void(*callback_fn) (int, void*);


    class Config {
    public:
        Config(std::string &config_filepath);
        ~Config();

    public:
        /* logging */
        const std::string& logging_filepath() { return logging_filepath_; }
        const int logging_level() { return logging_level_; }
        /* entry */
        const unsigned short entry_port() { return entry_port_; }
        /* foofs */
        const std::string& block_path() { return block_path_; }
        const std::string& block_filepath() { return block_filepath_; }
        const std::string& file_filepath() { return file_filepath_; }
        /* p2pnet */
        const unsigned short p2pnet_port() { return p2pnet_port_; }
        const std::string& p2pnet_filepath() { return p2pnet_filepath_; }
        const int rsa_key_length() { return rsa_key_length_; }
        const std::string& rsa_password() { return rsa_password_; }
        const std::string& rsa_public_key_path() { return rsa_public_key_path_; }
        const std::string& rsa_private_key_path() { return rsa_private_key_path_; }
        
    private:
        void GenConfig();

    private:
        boost::property_tree::ptree pt_;

        /* config file path, enter of config */
        std::string config_filepath_;

        /***** logger config *****/
        /* logging path */
        std::string logging_filepath_;
        int logging_level_;

        /***** entry config *****/
        unsigned short entry_port_;

        /***** foofs config *****/
        /* block path for saving block-files */
        std::string block_path_;
        /* block-files mapping path */
        std::string block_filepath_;
        /* files mapping path to save metadata */
        std::string file_filepath_;

        /***** p2pnet config *****/
        unsigned short p2pnet_port_;
        /* p2pnet config file path */
        std::string p2pnet_filepath_;
        int rsa_key_length_;
        std::string rsa_password_;
        std::string rsa_public_key_path_;
        std::string rsa_private_key_path_;

    };
}

#endif // !MANDIS_MANDIS_CONFIG_H_

