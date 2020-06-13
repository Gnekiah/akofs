#include "../include/foofs.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include "../include/hashlib.h"

namespace foofs {

    FooFS::FooFS(config::Config *config, p2pnet::P2Pnet *p2pnet, logger::Logger *logger)
        : config_(config),
        p2pnet_(p2pnet), 
        logger_(logger)
    {
        cbfn = this;

        int ret = 0;
        char buffer[1024];
        std::vector<std::string> vec;

        std::ifstream fin(config_->block_filepath());
        while (fin) {
            fin.getline(buffer, sizeof(buffer));
            vec.clear();
            ret = Split(vec, buffer, '|');
            if (ret != Block::NR_ATTRIBUTE || ret == 0) {
                LOG_ERROR(logger_, "block-info-file Error");
                LOG_TRACE(logger_, boost::lexical_cast<std::string>(vec.size()));
                LOG_TRACE(logger_, buffer);
                if (ret != 0)
                    assert(0);
                continue;
            }

            std::set<std::string>::iterator iter = block_set_.find(vec[0]);
            if (iter != block_set_.end()) {
                // message: current block existed
                continue;
            }

            Block *block = new Block(vec[0], Str2Int(vec[1]), Str2Int(vec[2]));
            hash_to_block_.insert(std::pair<std::string, Block*>(block->hash_key(), block));
            block_set_.insert(block->hash_key());
            block_vec_.push_back(block);
        }
        fin.close();
        
        fin.open(config_->file_filepath());
        while (fin) {
            fin.getline(buffer, sizeof(buffer));
            vec.clear();
            ret = Split(vec, buffer, '|');
            if (ret != File::NR_ATTRIBUTE || ret == 0) {
                LOG_ERROR(logger_, "file-info-file Error");
                LOG_TRACE(logger_, boost::lexical_cast<std::string>(vec.size()));
                LOG_TRACE(logger_, buffer);
                if (ret != 0)
                    assert(0);
                continue;
            }

            File *file = new File(vec[0], vec[1], vec[2]);
            name_to_file_.insert(std::pair<std::string, File*>(file->file_name(), file));
            hash_to_file_.insert(std::pair<std::string, File*>(file->hash_key(), file));
            file_vec_.push_back(file);
        }
        fin.close();
    }

    FooFS::~FooFS() {
        for (auto iter = block_vec_.begin(); iter != block_vec_.end(); iter++)
            delete (*iter);
        for (auto iter = file_vec_.begin(); iter != file_vec_.end(); iter++)
            delete (*iter);
    }

    int FooFS::Write(std::string &filepath) {
        std::ifstream fin(filepath, std::ios::binary);
        if (!fin) {
            LOG_WARNING(logger_, "File Not Found");
            return -1;
        }

        std::vector<Block*> blocks_for_append_vec;
        File *file_for_append = nullptr;

        std::string buffer_key;
        int buffer_size, buffer_flag;
        int block_cnt = 0;
        std::vector<std::string> buffer_key_vec;
        
        while (fin) {
            fin.read(buffer_.data(), 1024 * 512);
            buffer_size = fin.gcount();
            if (buffer_size <= 0) {
                assert(0);
                continue;
            }

            buffer_key = hashlib::Sha1::GetSha1(buffer_.data(), buffer_size);
            buffer_flag = 0;
            buffer_key_vec.push_back(buffer_key);

            std::set<std::string>::iterator iter = block_set_.find(buffer_key);
            if (iter != block_set_.end()) {
                LOG_DEBUG(logger_, "Block Key Existed!");
                continue;
            }

            boost::filesystem::path out_path = boost::filesystem::path(config_->block_path()) / buffer_key;
            std::ofstream fout(out_path.string(), std::ios::binary);
            fout.write(buffer_.data(), buffer_size);
            fout.close();
            block_cnt++;

            Block *block = new Block(buffer_key, buffer_size, buffer_flag);
            hash_to_block_.insert(std::pair<std::string, Block*>(block->hash_key(), block));
            block_set_.insert(block->hash_key());
            block_vec_.push_back(block);
            ///for update block file
            blocks_for_append_vec.push_back(block);
        }
        fin.close();

        LOG_INFO(logger_, "Write Blocks= " + boost::lexical_cast<std::string>(block_cnt));

        std::stringstream ss;
        for (auto iter = buffer_key_vec.begin(); iter != buffer_key_vec.end(); iter++) 
            ss << (*iter) << std::endl;
        strcpy(buffer_.data(), ss.str().c_str());
        buffer_size = (int)strlen(buffer_.data());
        buffer_key = hashlib::Sha1::GetSha1(buffer_.data(), buffer_size);
        buffer_flag = 1;

        std::set<std::string>::iterator iter = block_set_.find(buffer_key);
        ///while file not existed
        if (iter == block_set_.end()) {
            boost::filesystem::path out_path = boost::filesystem::path(config_->block_path()) / buffer_key;
            std::ofstream fout(out_path.string(), std::ios::binary);
            fout.write(buffer_.data(), buffer_size);
            fout.close();

            Block *block = new Block(buffer_key, buffer_size, buffer_flag);
            hash_to_block_.insert(std::pair<std::string, Block*>(block->hash_key(), block));
            block_set_.insert(block->hash_key());
            block_vec_.push_back(block);

            std::string file_name = boost::filesystem::path(filepath).filename().string();
            std::string description = "file";
            File *file = new File(file_name, description, buffer_key);
            name_to_file_.insert(std::pair<std::string, File*>(file->file_name(), file));
            hash_to_file_.insert(std::pair<std::string, File*>(file->hash_key(), file));
            file_vec_.push_back(file);

            file_for_append = file;
            blocks_for_append_vec.push_back(block);
        }
        ///write blocks_for_append_vec to file
        std::ofstream fout(config_->block_filepath(), std::ios::app);
        for (auto iter = blocks_for_append_vec.begin(); iter != blocks_for_append_vec.end(); iter++) {
            std::stringstream ss;
            ss << (*iter)->hash_key() << "|" << (*iter)->buffer_size() << "|" << (*iter)->block_flag() << std::endl;
            fout.write(ss.str().c_str(), ss.str().length());

            boost::filesystem::path out_path = boost::filesystem::path(config_->block_path()) / (*iter)->hash_key();
            ///store to remote
            p2pnet_->ReqStore((*iter)->hash_key(), out_path.string());
        }
        fout.close();

        if (file_for_append) {
            std::stringstream ss;
            ss << file_for_append->file_name() << "|" << file_for_append->description() << "|" 
                << file_for_append->hash_key() << std::endl;
            fout.open(config_->file_filepath(), std::ios::app);
            fout.write(ss.str().c_str(), ss.str().length());
            fout.close();
            ///sync to remote
            p2pnet_->ReqSync(ss.str());
        }

        return 0;
    }

    int FooFS::ReadByName(std::string &file_name, std::string &dest_path) {
        File *file = (*name_to_file_.find(file_name)).second;
        if (file == nullptr)
            return -1;

        std::vector<std::string> hash_vec;

        dest_path = (boost::filesystem::path(dest_path) / file_name).string();
        std::ifstream fin((boost::filesystem::path(config_->block_path()) / file->hash_key()).string());
        if (!fin) {
            LOG_TRACE(logger_, "File Not Found On Local, Try To Get From P2Pnet");
            /// Get Block From P2Pnet
            p2pnet_->ReqAccess(file->hash_key());
        }

        LOG_TRACE(logger_, "Read File From: " + file_name + " To: " + dest_path);
        int ret = 0;
        int buffer_size;
        while (fin) {
            fin.getline(buffer_.data(), 1024 * 512);
            if (buffer_.data()[0] == '\0')
                break;
            hash_vec.push_back(buffer_.data());
            std::string block_path = (boost::filesystem::path(config_->block_path()) / buffer_.data()).string();
            if (!boost::filesystem::exists(block_path)) {
                /// Get Block From P2Pnet
                p2pnet_->ReqAccess(buffer_.data());
            }
        }
        fin.close();

        if (boost::filesystem::exists(dest_path)) {
            LOG_WARNING(logger_, "Destinct File Exists");
            return -2;
        }

        std::ofstream fout(dest_path, std::ios::app | std::ios::binary);
        for (auto iter = hash_vec.begin(); iter != hash_vec.end(); iter++) {
            std::string block_path = (boost::filesystem::path(config_->block_path()) / (*iter)).string();
            fin.open(block_path, std::ios::binary);
            fin.read(buffer_.data(), 1024 * 512);
            buffer_size = fin.gcount();
            fin.close();
            fout.write(buffer_.data(), buffer_size);
        }
        fout.close();
        return 0;
    }
    
    int FooFS::ReadByHash(std::string &file_hash) {
        File *file = (*hash_to_file_.find(file_hash)).second;
        if (file == nullptr)
            return -1;
        // TODO: not use
        assert(0);
        return 0;
    }

    int FooFS::Delete() {

        return 0;
    }

    int FooFS::ReadMetaData(char* buffer) {
        std::stringstream ss;
        ss << "Hash Key                                    File Name" << std::endl;
        for (auto iter = file_vec_.begin(); iter != file_vec_.end(); iter++) 
            ss << (*iter)->hash_key() << "    " << (*iter)->file_name() << std::endl;

        strcpy(buffer, ss.str().c_str());
        return (int)ss.str().length();
    }

    void FooFS::Run() {

    }

    void FooFS::Start() {

    }

    void FooFS::Stop() {

    }

    void FooFS::Join() {

    }


    int FooFS::Split(std::vector<std::string> &vec, const std::string &str, const char pattern)
    {
        std::stringstream input(str);
        std::string temp;
        while (getline(input, temp, pattern))
        {
            vec.push_back(temp);
        }
        return (int)vec.size();
    }

    int FooFS::Str2Int(std::string &str) {
        int ret = 0;
        std::istringstream iss(str);
        iss >> ret;
        return ret;
    }

    ///for callback
    void CallBack(int cbtype, void* ptr) {
        FooFS *foofs = (FooFS*)cbfn;
        //foofs->DispatchSpi(cbtype, ptr);
    }
}