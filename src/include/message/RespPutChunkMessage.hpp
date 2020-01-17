#ifndef SPARKLE_MESSAGE_RESPONSE_PUT_CHUNK_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_PUT_CHUNK_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespPutChunkMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    RespPutChunkMessage(const RespPutChunkMessage&) = delete;
    RespPutChunkMessage(RespPutChunkMessage&&) = delete;
    RespPutChunkMessage& operator=(const RespPutChunkMessage&) = delete;
    RespPutChunkMessage& operator=(RespPutChunkMessage&&) = delete;
    RespPutChunkMessage()
    {

    }

    ~RespPutChunkMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_PUT_CHUNK_MESSAGE_HPP_
