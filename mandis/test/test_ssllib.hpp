#ifndef MANDIS_TEST_SSLLIB_HPP_
#define MANDIS_TEST_SSLLIB_HPP_

#include <boost/filesystem.hpp>

#include "../include/ssllib.h"
#include "../include/logger.h"
#include "../include/timelib.h"

std::string TestRsaPairCase1(logger::Logger *log, std::string &msg) {
    std::string priv_path = (boost::filesystem::temp_directory_path() / "id_rsa").string();
    std::string pub_path = (boost::filesystem::temp_directory_path() / "id_rsa.pub").string();
    std::string password("passwd123456");
    ssllib::RsaPair rsapair(priv_path, pub_path, password, log);
    ssllib::RsaPubkey pubkey(pub_path, log);
    
    unsigned char plain[2048];
    unsigned char cipher[2048];
    int buff_len = rsapair.Sign((unsigned char*)msg.c_str(), msg.length(), cipher);
    buff_len = pubkey.Verify(cipher, buff_len, plain);
    plain[buff_len] = '\0';
    std::string ret((char*)plain);
    return ret;
}

std::string TestRsaPairCase2(logger::Logger *log, std::string &msg) {
    std::string priv_path = (boost::filesystem::temp_directory_path() / "id_rsa").string();
    std::string pub_path = (boost::filesystem::temp_directory_path() / "id_rsa.pub").string();
    std::string password("passwd123456");
    ssllib::RsaPair rsapair(priv_path, pub_path, password, log);

    unsigned char buff[2048];
    int buff_len_ = rsapair.GetPublicKeyBytes(buff);
    ssllib::RsaPubkey pubkey(buff, buff_len_, log);

    unsigned char plain[2048];
    unsigned char cipher[2048];
    int buff_len = rsapair.Sign((unsigned char*)msg.c_str(), msg.length(), cipher);
    buff_len = pubkey.Verify(cipher, buff_len, plain);
    plain[buff_len] = '\0';
    std::string ret((char*)plain);
    return ret;
}

std::string TestRsaPairCase3(logger::Logger *log, std::string &msg) {
    std::string priv_path = (boost::filesystem::temp_directory_path() / "id_rsa").string();
    std::string pub_path = (boost::filesystem::temp_directory_path() / "id_rsa.pub").string();
    std::string password("passwd123456");
    ssllib::RsaPair rsapair(priv_path, pub_path, password, log);
    ssllib::RsaPubkey pubkey(pub_path, log);

    unsigned char plain[2048];
    unsigned char cipher[2048];
    int buff_len = pubkey.Encrypt((unsigned char *)msg.c_str(), msg.length(), cipher);
    buff_len = rsapair.Decrypt(cipher, buff_len, plain);
    plain[buff_len] = '\0';
    std::string ret((char*)plain);
    return ret;
}

std::string TestRsaPairCase4(logger::Logger *log, std::string &msg) {
    std::string priv_path = (boost::filesystem::temp_directory_path() / "id_rsa").string();
    std::string pub_path = (boost::filesystem::temp_directory_path() / "id_rsa.pub").string();
    std::string password("passwd123456");
    ssllib::RsaPair rsapair(priv_path, pub_path, password, log);

    unsigned char buff[2048];
    int buff_len_ = rsapair.GetPublicKeyBytes(buff);
    ssllib::RsaPubkey pubkey(buff, buff_len_, log);

    unsigned char plain[2048];
    unsigned char cipher[2048];
    int buff_len = pubkey.Encrypt((unsigned char *)msg.c_str(), msg.length(), cipher);
    buff_len = rsapair.Decrypt(cipher, buff_len, plain);
    plain[buff_len] = '\0';
    std::string ret((char*)plain);
    return ret;
}

std::string TestRsaPairCase5(logger::Logger *log, std::string &msg) {
    std::string priv_path = (boost::filesystem::temp_directory_path() / "id_rsa").string();
    std::string pub_path = (boost::filesystem::temp_directory_path() / "id_rsa.pub").string();
    std::string password("passwd123456");
    ssllib::RsaPair rsapair(priv_path, pub_path, password, log);

    unsigned char buff[2048];
    int buff_len_ = rsapair.GetPublicKeyBytes(buff);
    ssllib::RsaPubkey pubkey(buff, buff_len_, log);

    unsigned char buff2[2048];
    int buff_len2_ = pubkey.GetPublicKeyBytes(buff2);
    ssllib::RsaPubkey pubkey2(buff2, buff_len2_, log);

    std::string pub2_path = (boost::filesystem::temp_directory_path() / "id_rsa2.pub").string();
    pubkey2.Save(pub2_path);

    unsigned char plain[2048];
    unsigned char cipher[2048];
    int buff_len = pubkey2.Encrypt((unsigned char *)msg.c_str(), msg.length(), cipher);
    buff_len = rsapair.Decrypt(cipher, buff_len, plain);
    plain[buff_len] = '\0';
    std::string ret((char*)plain);
    return ret;
}

#endif // !MANDIS_TEST_SSLLIB_HPP_

