#include "../include/ssllib.h"

#include <boost/filesystem.hpp>

#include "../include/magic.h"

namespace ssllib {
    RsaPair::RsaPair(const std::string &priv_path, const std::string &pub_path, const std::string &password,
        logger::Logger *logger, int rsa_key_len)
        : private_key_path_(priv_path), public_key_path_(pub_path), password_(password),
        logger_(logger), rsa_key_length_(rsa_key_len) 
    {
#ifdef __linux__
        return;
#endif

        boost::filesystem::path privpath(priv_path);
        boost::filesystem::path pubpath(pub_path);

        /* Load keypair from files. */
        if (boost::filesystem::exists(privpath) && boost::filesystem::exists(pubpath)) {
            BIO * priv_bio = BIO_new_file(priv_path.c_str(), "r");
            BIO * pub_bio = BIO_new_file(pub_path.c_str(), "r");
            LOG_INFO(logger_, "Load keypair from files!");
            if (!PEM_read_bio_RSAPrivateKey(priv_bio, &private_key_, NULL, NULL)) {
                LOG_ERROR(logger_, "Failed to read rsa private key!");
                ExitFlag = true;
            }
            if (!PEM_read_bio_RSAPublicKey(pub_bio, &public_key_, NULL, NULL)) {
                LOG_ERROR(logger_, "Failed to read rsa public key!");
                ExitFlag = true;
            }

            BIO_free(priv_bio);
            BIO_free(pub_bio);
            return;
        }
        else if (boost::filesystem::exists(privpath) || boost::filesystem::exists(pubpath)) {
            LOG_ERROR(logger_, "Rsa keys unmatched!");
            ExitFlag = true;
            return;
        }

        /* Generate keypair. */
        LOG_INFO(logger_, "Generate keypair!");
        RSA * rsa = RSA_generate_key(rsa_key_len, RSA_F4, NULL, NULL);
        if (!rsa) {
            LOG_ERROR(logger_, "Failed to generate rsa keys!");
            ExitFlag = true;
            return;
        }

        BIO * priv_bio = BIO_new_file(priv_path.c_str(), "w");
        BIO * pub_bio = BIO_new_file(pub_path.c_str(), "w");
        if (PEM_write_bio_RSAPrivateKey(priv_bio, rsa, NULL, NULL, 0, NULL, NULL) <= 0) {
            LOG_ERROR(logger_, "Failed to write private keys!");
            ExitFlag = true;
        }
        if (PEM_write_bio_RSAPublicKey(pub_bio, rsa) <= 0) {
            LOG_ERROR(logger_, "Failed to write public keys!");
            ExitFlag = true;
        }

        BIO_free(priv_bio);
        BIO_free(pub_bio);
        private_key_ = RSAPrivateKey_dup(rsa);
        public_key_ = RSAPublicKey_dup(rsa);
    }

    RsaPair::~RsaPair() {
        if (public_key_)
            RSA_free(public_key_);
        if (private_key_)
            RSA_free(private_key_);
    }

    int RsaPair::GetPublicKeyBytes(unsigned char *buff) {
        unsigned char *p = buff;
        return i2d_RSAPublicKey(public_key_, &p);
    }

    int RsaPair::GetPrivateKeyBytes(unsigned char *buff) {
        unsigned char *p = buff;
        return i2d_RSAPrivateKey(private_key_, &p);
    }

    int RsaPair::Encrypt(const unsigned char *plain, int plen, unsigned char *cipher) {
        return RSA_public_encrypt(plen, plain, cipher, public_key_, RSA_PKCS1_PADDING);
    }

    int RsaPair::Decrypt(const unsigned char *cipher, int clen, unsigned char *plain) {
        return RSA_private_decrypt(clen, cipher, plain, private_key_, RSA_PKCS1_PADDING);    
    }

    int RsaPair::Sign(const unsigned char *plain, int plen, unsigned char *cipher) {
        return RSA_private_encrypt(plen, plain, cipher, private_key_, RSA_PKCS1_PADDING);
    }

    int RsaPair::Verify(const unsigned char *cipher, int clen, unsigned char *plain) {
        return RSA_public_decrypt(clen, cipher, plain, public_key_, RSA_PKCS1_PADDING);
    }


    RsaPubkey::RsaPubkey(std::string &pub_path, logger::Logger *logger)
        : logger_(logger) {
        boost::filesystem::path pubpath(pub_path);
        if (!boost::filesystem::exists(pubpath)) {
            LOG_WARNING(logger_, "File not found!");
            throw std::exception();
            return;
        }
        BIO * pub_bio = BIO_new_file(pub_path.c_str(), "r");

        LOG_TRACE(logger_, "Load public key from file!");
        if (!PEM_read_bio_RSAPublicKey(pub_bio, &public_key_, NULL, NULL)) {
            LOG_WARNING(logger_, "Failed to load public key!");
            throw std::exception();
        }
        BIO_free(pub_bio);
    }

    RsaPubkey::RsaPubkey(unsigned char *pubkey, int len, logger::Logger *logger)
        : logger_(logger) {
        const unsigned char *p = pubkey;
        public_key_ = d2i_RSAPublicKey(NULL, &p, len);
    }

    RsaPubkey::~RsaPubkey() {
        if (public_key_)
            RSA_free(public_key_);
    }

    int RsaPubkey::Encrypt(const unsigned char *plain, int plen, unsigned char *cipher) {
        return RSA_public_encrypt(plen, plain, cipher, public_key_, RSA_PKCS1_PADDING);
    }

    int RsaPubkey::Verify(const unsigned char *cipher, int clen, unsigned char *plain) {
        return RSA_public_decrypt(clen, cipher, plain, public_key_, RSA_PKCS1_PADDING);
    }

    int RsaPubkey::Save(std::string &save_path) {
        BIO * pub_bio = BIO_new_file(save_path.c_str(), "w");
        if (PEM_write_bio_RSAPublicKey(pub_bio, public_key_) <= 0) {
            LOG_ERROR(logger_, "Failed to write public keys!");
            return -1;
        }
        BIO_free(pub_bio);
        return 0;
    }

    int RsaPubkey::GetPublicKeyBytes(unsigned char *buff) {
        unsigned char *p = buff;
        return i2d_RSAPublicKey(public_key_, &p);
    }
}
