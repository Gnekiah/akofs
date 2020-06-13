#ifndef MANDIS_MANDIS_BLOCK_H_
#define MANDIS_MANDIS_BLOCK_H_

#include <string>

namespace foofs {

    class Block {
    public:
        Block(std::string hash_key, int buffer_size, int block_flag);
        ~Block();

        const std::string& hash_key() { return hash_key_; }
        const int buffer_size() { return buffer_size_; }
        const int block_flag() { return block_flag_; }

    public:
        const static int NR_ATTRIBUTE = 3;

    private:
        std::string hash_key_;
        /* block size */
        int buffer_size_;
        /* block attrs */
        int block_flag_;
        /* counter of block, if cnter=0, then delete the block */
        int block_cnter_;   /* not used */
    };

}

#endif // !MANDIS_MANDIS_BLOCK_H_

