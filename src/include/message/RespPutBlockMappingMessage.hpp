#ifndef SPARKLE_MESSAGE_RESPONSE_PUT_BLOCK_MAPPING_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_PUT_BLOCK_MAPPING_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespPutBlockMappingMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    RespPutBlockMappingMessage(const RespPutBlockMappingMessage&) = delete;
    RespPutBlockMappingMessage(RespPutBlockMappingMessage&&) = delete;
    RespPutBlockMappingMessage& operator=(const RespPutBlockMappingMessage&) = delete;
    RespPutBlockMappingMessage& operator=(RespPutBlockMappingMessage&&) = delete;
    RespPutBlockMappingMessage()
    {

    }

    ~RespPutBlockMappingMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_PUT_BLOCK_MAPPING_MESSAGE_HPP_
