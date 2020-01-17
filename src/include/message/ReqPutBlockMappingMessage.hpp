#ifndef SPARKLE_MESSAGE_REQUEST_PUT_BLOCK_MAPPING_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_PUT_BLOCK_MAPPING_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqPutBlockMappingMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    ReqPutBlockMappingMessage(const ReqPutBlockMappingMessage&) = delete;
    ReqPutBlockMappingMessage(ReqPutBlockMappingMessage&&) = delete;
    ReqPutBlockMappingMessage& operator=(const ReqPutBlockMappingMessage&) = delete;
    ReqPutBlockMappingMessage& operator=(ReqPutBlockMappingMessage&&) = delete;
    ReqPutBlockMappingMessage()
    {

    }

    ~ReqPutBlockMappingMessage()
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

#endif // SPARKLE_MESSAGE_REQUEST_PUT_BLOCK_MAPPING_MESSAGE_HPP_
