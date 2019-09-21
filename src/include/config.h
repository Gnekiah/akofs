#ifndef SPARKLE_COMMON_CONFIG_
#define SPARKLE_COMMON_CONFIG_

#include <string>
#include <map>
#include <iostream>
#include <macros.h>

namespace sparkle {

    class Config {
    public:
        explicit Config(const std::string& path, std::string delimiter = "=", 
            std::string comment = "#");
        explicit Config();
        ~Config();

        template<class T> 
        T Read(const std::string& key) const;
        template<class T> 
        T Read(const std::string& key, const std::string& val) const;
        template<class T> 
        bool ReadInto(T& dst, const std::string& key) const;
        template<class T> 
        bool ReadInto(T& dst, const std::string& key, const std::string& val) const;

        template<class T>
        void Add(const std::string& key, const std::string& val);
        void Remove(const std::string& key);

        void ReadFile(const std::string& path, std::string delimiter = "=", 
            std::string comment = "#");
        bool FileExist(const std::string& path) const;
        bool KeyExist(const std::string& key) const;

        std::string delimiter() const { return delimiter_; }
        std::string comment() const { return comment_; }
        void set_delimiter(const std::string& delimiter) { delimiter_ = delimiter; }
        void set_comment(const std::string& comment) { comment_ = comment; }

        friend std::ostream& operator << (std::ostream& os, const Config& config);
        friend std::istream& operator >> (std::istream& is, Config& config);

    protected:
        template<class T> 
        static std::string Type2Str(const T& t);
        template<class T>
        static T str2Type(const std::string& s);
       
    private:
        DISALLOW_COPY_AND_ASSIGN(Config);

        std::string delimiter_;
        std::string comment_;
        std::map<std::string, std::string> contents_;
    };
}

#endif  // SPARKLE_COMMON_CONFIG_