#ifndef SPARKLE_MESSAGE_REQUEST_GET_METADATA_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_GET_METADATA_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqGetMetadataMessage : public Message {

public:
    uint8_t key_type;
    uint64_t obj_hash_key_length;
    uint8_t* obj_hash_key;

    /* TODO: add other header or data segments */

public:
    ReqGetMetadataMessage(const ReqGetMetadataMessage&) = delete;
    ReqGetMetadataMessage(ReqGetMetadataMessage&&) = delete;
    ReqGetMetadataMessage& operator=(const ReqGetMetadataMessage&) = delete;
    ReqGetMetadataMessage& operator=(ReqGetMetadataMessage&&) = delete;
    ReqGetMetadataMessage() :
        key_type(0),
        obj_hash_key_length(0),
        obj_hash_key(nullptr)
    {

    }

    ~ReqGetMetadataMessage()
    {
        delete obj_hash_key;
    }

    int Dump()
    {
        int err = 0;

        return err;
    }

    int Pack()
    {
        int err = 0;

        return err;
    }
};

#endif // SPARKLE_MESSAGE_REQUEST_GET_METADATA_MESSAGE_HPP_
