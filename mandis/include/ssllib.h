#ifndef MANDIS_SSLLIB_H_
#define MANDIS_SSLLIB_H_

#include <string>

#include "logger.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>

namespace ssllib {
    class RsaPair {
    public:
        RsaPair(const std::string &priv_path, const std::string &pub_path, const std::string &password,
            logger::Logger *logger, int ras_key_len=2048);
        ~RsaPair();

        int Encrypt(const unsigned char *plain, int plen, unsigned char *cipher);
        int Decrypt(const unsigned char *cipher, int clen, unsigned char *plain);
        int Sign(const unsigned char *plain, int plen, unsigned char *cipher);
        int Verify(const unsigned char *cipher, int clen, unsigned char *plain);

        int GetPublicKeyBytes(unsigned char *buff);
        int GetPrivateKeyBytes(unsigned char *buff);

    private:
        RSA * public_key_ = nullptr;
        RSA * private_key_ = nullptr;
        logger::Logger *logger_ = nullptr;

        int rsa_key_length_;
        std::string password_;
        std::string public_key_path_;
        std::string private_key_path_;
    };

    class RsaPubkey {
    public:
        RsaPubkey(std::string &pub_path, logger::Logger *logger);
        RsaPubkey(unsigned char *pubkey, int len, logger::Logger *logger);
        ~RsaPubkey();

        int Encrypt(const unsigned char *plain, int plen, unsigned char *cipher);
        int Verify(const unsigned char *cipher, int clen, unsigned char *plain);

        int Save(std::string &save_path);
        int GetPublicKeyBytes(unsigned char *buff);

    private:
        RSA * public_key_ = nullptr;
        logger::Logger *logger_ = nullptr;
    };
}

#endif // !MANDIS_SSLLIB_H_

