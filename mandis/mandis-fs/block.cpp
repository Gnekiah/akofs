#include "block.h"

namespace foofs {

    Block::Block(std::string hash_key, int buffer_size, int block_flag) 
        : hash_key_(hash_key),
        buffer_size_(buffer_size),
        block_flag_(block_flag)
    {

    }

    Block::~Block() {

    }

}