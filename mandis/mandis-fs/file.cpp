#include "file.h"

namespace foofs {

    File::File(std::string file_name, std::string description, std::string hash_key)
        : file_name_(file_name),
        description_(description),
        hash_key_(hash_key)
    {

    }

    File::~File() {

    }

}