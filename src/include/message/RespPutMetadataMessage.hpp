#ifndef SPARKLE_MESSAGE_RESPONSE_PUT_METADATA_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_PUT_METADATA_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespPutMetadataMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    RespPutMetadataMessage(const RespPutMetadataMessage&) = delete;
    RespPutMetadataMessage(RespPutMetadataMessage&&) = delete;
    RespPutMetadataMessage& operator=(const RespPutMetadataMessage&) = delete;
    RespPutMetadataMessage& operator=(RespPutMetadataMessage&&) = delete;
    RespPutMetadataMessage()
    {

    }

    ~RespPutMetadataMessage()
    {

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

#endif // SPARKLE_MESSAGE_RESPONSE_PUT_METADATA_MESSAGE_HPP_
