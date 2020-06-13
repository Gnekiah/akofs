#ifndef MANDIS_MANDIS_FOOFS_H_
#define MANDIS_MANDIS_FOOFS_H_

#include <set>
#include <map>
#include <vector>

#include "../include/logger.h"
#include "../include/config.h"
#include "../include/p2pnet.h"
#include "../mandis-fs/file.h"
#include "../mandis-fs/block.h"

namespace foofs {

    static volatile void* cbfn = nullptr;
    void CallBack(int cbtype, void* ptr);

    class FooFS {
    public:
        FooFS(config::Config *config, p2pnet::P2Pnet *p2pnet, logger::Logger *logger);
        ~FooFS();

        int Write(std::string &filepath);
        int ReadByName(std::string &file_name, std::string &dest_path);
        int ReadByHash(std::string &filehash);
        int Delete();
        int ReadMetaData(char* buffer);

        void Run();
        void Start();
        void Stop();
        void Join();
        
        int Split(std::vector<std::string> &vec, const std::string &str, const char pattern);
        int Str2Int(std::string &str);

    private:
        std::map<std::string, Block*> hash_to_block_;
        std::set<std::string> block_set_;
        std::vector<Block*> block_vec_;

        std::map<std::string, File*> name_to_file_;
        std::map<std::string, File*> hash_to_file_;
        std::set<std::string> file_set_;    /* not used, assume not repeating file */
        std::vector<File*> file_vec_;

        boost::array<char, 1024 * 512> buffer_;

        config::Config *config_ = nullptr;
        p2pnet::P2Pnet *p2pnet_ = nullptr;
        logger::Logger *logger_ = nullptr;
    };
}

#endif // !#define MANDIS_MANDIS_FOOFS_H_

