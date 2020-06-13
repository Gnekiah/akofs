#include "../include/hashlib.h"

#include <boost/uuid/sha1.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>


namespace hashlib {

    std::string Sha1::GetSha1(const char *msg, size_t len) {
        uint32_t sha1[5];
        boost::uuids::detail::sha1 s;
        std::stringstream convertor;
        s.process_bytes(msg, len);
        s.get_digest(sha1);

        for (int i = 0; i < 5; i++)
            convertor << std::hex << std::setw(8) << std::setfill('0') << sha1[i];
        return convertor.str();
    }

    std::string Sha1::GetSha1(const unsigned char *msg, size_t len) {
        uint32_t sha1[5];
        boost::uuids::detail::sha1 s;
        std::stringstream convertor;
        s.process_bytes(msg, len);
        s.get_digest(sha1);

        for (int i = 0; i < 5; i++)
            convertor << std::hex << std::setw(8) << std::setfill('0') << sha1[i];
        return convertor.str();
    }

    std::string Sha1::GetSha1(const std::string &str) {
        return GetSha1(str.c_str(), str.length());
    }

    int Sha1::GetSha1(const char *msg, size_t len, DigestField &bytes) {
        boost::uuids::detail::sha1 s;
        std::stringstream convertor;
        s.process_bytes(msg, len);
        s.get_digest(bytes.digest);
        return 0;
    }

    int Sha1::GetSha1(const unsigned char *msg, size_t len, DigestField &bytes) {
        boost::uuids::detail::sha1 s;
        std::stringstream convertor;
        s.process_bytes(msg, len);
        s.get_digest(bytes.digest);
        return 0;
    }

    int Sha1::GetSha1(const std::string &str, DigestField &bytes) {
        return GetSha1(str.c_str(), str.length(), bytes);
    }

    int Sha1::IsEqual(const DigestField &d1, const DigestField &d2) {
        for (int i = 0; i < 5; i++) {
            if (d1.digest[i] != d2.digest[i])
                return 0;
        }
        return 1;
    }

    std::string Sha1::Bytes2Str(const DigestField &bytes) {
        std::stringstream convertor;
        for (int i = 0; i < 5; i++)
            convertor << std::hex << std::setw(8) << std::setfill('0') << bytes.digest[i];
        return convertor.str();
    }

    int Sha1::Str2Bytes(const std::string &str, DigestField &bytes) {
        std::stringstream convertor;
        convertor << std::hex;
        for (int i = 0; i < 5; i++) {
            convertor.clear();
            convertor << str.substr(i * 8, 8);
            std::string tmp;
            convertor >> bytes.digest[i];
        }
        return 0;
    }

    std::string Sha1::XorMetric(const std::string &s1, const std::string &s2) {
        DigestField d1, d2, s;
        Str2Bytes(s1, d1);
        Str2Bytes(s2, d2);
        XorMetric(d1, d2, s);
        return Bytes2Str(s);
    }

    std::string Sha1::XorMetric(const std::string &s1, const DigestField &s2) {
        DigestField d1, s;
        Str2Bytes(s1, d1);
        XorMetric(d1, s2, s);
        return Bytes2Str(s);
    }

    std::string Sha1::XorMetric(const DigestField &s1, const DigestField &s2) {
        DigestField s;
        XorMetric(s1, s2, s);
        return Bytes2Str(s);
    }

    int Sha1::XorMetric(const std::string &s1, const std::string &s2, DigestField &s) {
        DigestField d1, d2;
        Str2Bytes(s1, d1);
        Str2Bytes(s2, d2);
        return XorMetric(d1, d2, s);

    }

    int Sha1::XorMetric(const std::string &s1, const DigestField &s2, DigestField &s) {
        DigestField d1;
        Str2Bytes(s1, d1);
        return XorMetric(d1, s2, s);
    }

    int Sha1::XorMetric(const DigestField &s1, const DigestField &s2, DigestField &s) {
        for (int i = 0; i < 5; i++) {
            s.digest[i] = s1.digest[i] ^ s2.digest[i];
        }
        return 0;
    }
}
