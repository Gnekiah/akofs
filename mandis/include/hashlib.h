#ifndef MANDIS_HASHLIB_H_
#define MANDIS_HASHLIB_H_

#include <cstdint>
#include <string>

namespace hashlib {

    typedef struct DigestField {
        uint32_t digest[5];
    } DigestField;
        
    class Sha1 {
    public:
        static std::string GetSha1(const char *msg, size_t len);
        static std::string GetSha1(const unsigned char *msg, size_t len);
        static std::string GetSha1(const std::string &str);
        static int GetSha1(const char *msg, size_t len, DigestField &bytes);
        static int GetSha1(const unsigned char *msg, size_t len, DigestField &bytes);
        static int GetSha1(const std::string &str, DigestField &bytes);

        static int IsEqual(const DigestField &d1, const DigestField &d2);

        static std::string Bytes2Str(const DigestField &bytes);
        static int Str2Bytes(const std::string &str, DigestField &bytes);

        static std::string XorMetric(const std::string &s1, const std::string &s2);
        static std::string XorMetric(const std::string &s1, const DigestField &s2);
        static std::string XorMetric(const DigestField &s1, const DigestField &s2);
        static int XorMetric(const std::string &s1, const std::string &s2, DigestField &s);
        static int XorMetric(const std::string &s1, const DigestField &s2, DigestField &s);
        static int XorMetric(const DigestField &s1, const DigestField &s2, DigestField &s);
    };

} // namespace hashlib 

#endif // !MANDIS_HASHLIB_H_

