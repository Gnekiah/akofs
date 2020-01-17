#ifndef SPARKLE_MESSAGE_REQUEST_GET_BLOCK_MAPPING_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_GET_BLOCK_MAPPING_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqGetBlockMappingMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    ReqGetBlockMappingMessage(const ReqGetBlockMappingMessage&) = delete;
    ReqGetBlockMappingMessage(ReqGetBlockMappingMessage&&) = delete;
    ReqGetBlockMappingMessage& operator=(const ReqGetBlockMappingMessage&) = delete;
    ReqGetBlockMappingMessage& operator=(ReqGetBlockMappingMessage&&) = delete;
    ReqGetBlockMappingMessage()
    {

    }

    ~ReqGetBlockMappingMessage()
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

#endif // SPARKLE_MESSAGE_REQUEST_GET_BLOCK_MAPPING_MESSAGE_HPP_
