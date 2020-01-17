#ifndef SPARKLE_MESSAGE_RESPONSE_GET_BLOCK_MAPPING_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_GET_BLOCK_MAPPING_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespGetBlockMappingMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    RespGetBlockMappingMessage(const RespGetBlockMappingMessage&) = delete;
    RespGetBlockMappingMessage(RespGetBlockMappingMessage&&) = delete;
    RespGetBlockMappingMessage& operator=(const RespGetBlockMappingMessage&) = delete;
    RespGetBlockMappingMessage& operator=(RespGetBlockMappingMessage&&) = delete;
    RespGetBlockMappingMessage()
    {

    }

    ~RespGetBlockMappingMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_GET_BLOCK_MAPPING_MESSAGE_HPP_
