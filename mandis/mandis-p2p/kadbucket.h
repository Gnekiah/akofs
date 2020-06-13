#ifndef MANDIS_MANDIS_P2P_KAD_BUCKET_H_
#define MANDIS_MANDIS_P2P_KAD_BUCKET_H_

#include <string>
#include <vector>
#include "../include/logger.h"
#include "../include/hashlib.h"

namespace p2pnet {

    typedef struct KadNodeField {
        //hashlib::DigestField distance;
        hashlib::DigestField node_id;
        char protocol[5];
        char ip[17];
        uint16_t port;
    } KadNodeField; /* 64 bytes. */

    class KadBucket {
    public:
        KadBucket(logger::Logger * log, int max_size=160);
        ~KadBucket();

        // @brief: Filter nodes nearest to target
        // @param:
        //      container: nodes filted as result
        //      target: search nodes nearest to the target
        //      cnt: amount of cnt nodes returned
        // @return: count of nodes filted
        int FilterKadNode(std::vector<KadNodeField> &container, const hashlib::DigestField &target, int cnt);

        // @brief: Insert new node into kad bucket
        // @param:
        //      node: new node to be inserted
        // @return: 0 if success, else failed
        int InsertKadNode(KadNodeField &node);

        int PopFirstNode(KadNodeField &node);

    private:
        logger::Logger * logger_ = nullptr;
        std::vector<KadNodeField> *kad_bucket_ = nullptr;
        int max_size_;
    };
}


#endif // !MANDIS_MANDIS_P2P_KAD_BUCKET_H_

 