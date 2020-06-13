#ifndef MANDIS_MANDIS_FILE_H_
#define MANDIS_MANDIS_FILE_H_

#include <string>
#include <vector>

namespace foofs {

    class File {
    public:
        File(std::string file_name, std::string description, std::string hash_key);
        ~File();

        const std::string& file_name() { return file_name_; }
        const std::string& description() { return description_; }
        const std::string& hash_key() { return hash_key_; }

    public:
        const static int NR_ATTRIBUTE = 3;

    private:
        std::string file_name_;
        std::string description_;
        /* hash value of current file */
        std::string hash_key_;
    };
}

#endif // !MANDIS_MANDIS_FILE_H_

