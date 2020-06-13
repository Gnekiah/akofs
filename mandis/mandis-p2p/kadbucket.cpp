#include "kadbucket.h"
#include <map>

namespace p2pnet {
    KadBucket::KadBucket(logger::Logger * log, int max_size)
        : max_size_(max_size), logger_(log) {
        kad_bucket_ = new std::vector<KadNodeField>(max_size);
    }

    KadBucket::~KadBucket() {
        delete kad_bucket_;
        logger_ = nullptr;
    }

    int KadBucket::FilterKadNode(std::vector<KadNodeField> &container, const hashlib::DigestField &target, int cnt) {
        std::vector<KadNodeField>::iterator iter;
        if (cnt >= kad_bucket_->size()) {
            for (iter = kad_bucket_->begin(); iter != kad_bucket_->end(); iter++)
                container.push_back(*iter);
            return (int)kad_bucket_->size();
        }

        struct DigestCompare {
            bool operator () (const hashlib::DigestField &d1, const hashlib::DigestField &d2) const {
                for (int i = 0; i < 5; i++) {
                    if (d1.digest[i] == d2.digest[i])
                        continue;
                    return d1.digest[i] < d2.digest[i];
                }
                return false;
            }
        };

        std::map<hashlib::DigestField, KadNodeField, DigestCompare> map;
        for (iter = kad_bucket_->begin(); iter != kad_bucket_->end(); iter++) {
            hashlib::DigestField distance;
            hashlib::Sha1::XorMetric(target, (*iter).node_id, distance);
            map.insert(std::make_pair(distance, (*iter)));
        }
        std::map<hashlib::DigestField, KadNodeField>::iterator map_iter = map.begin();
        for (int i = 0; i < cnt, map_iter != map.end(); i++, map_iter++)
            container.push_back((*map_iter).second);
        return cnt;
    }

    int KadBucket::InsertKadNode(KadNodeField &node) {
        std::vector<KadNodeField>::iterator iter;
        for (iter = kad_bucket_->begin(); iter != kad_bucket_->end(); iter++) {
            if (hashlib::Sha1::IsEqual((*iter).node_id, node.node_id)) {
                kad_bucket_->erase(iter);
                kad_bucket_->push_back(node);
                return 0;
            }
        }
        if (kad_bucket_->size() < max_size_) {
            kad_bucket_->push_back(node);
            return 0;
        }
        return -1;
    }

    int KadBucket::PopFirstNode(KadNodeField &node) {
        node = kad_bucket_->front();
        kad_bucket_->erase(kad_bucket_->begin());
        return 0;
    }
}