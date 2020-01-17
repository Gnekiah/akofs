#ifndef SPARKLE_MESSAGE_RESPONSE_GET_METADATA_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_GET_METADATA_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespGetMetadataMessage : public Message {

public:
    uint16_t metadata_offset;
    uint64_t metadata_length;
    uint8_t* metadata;

    /* TODO: add other header or data segments */

public:
    RespGetMetadataMessage(const RespGetMetadataMessage&) = delete;
    RespGetMetadataMessage(RespGetMetadataMessage&&) = delete;
    RespGetMetadataMessage& operator=(const RespGetMetadataMessage&) = delete;
    RespGetMetadataMessage& operator=(RespGetMetadataMessage&&) = delete;
    RespGetMetadataMessage() :
        metadata_offset(0),
        metadata_length(0),
        metadata(nullptr)
    {

    }

    ~RespGetMetadataMessage()
    {
        delete metadata;
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

#endif // SPARKLE_MESSAGE_RESPONSE_GET_METADATA_MESSAGE_HPP_
