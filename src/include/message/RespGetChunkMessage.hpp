#ifndef SPARKLE_MESSAGE_RESPONSE_GET_CHUNK_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_GET_CHUNK_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespGetChunkMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    RespGetChunkMessage(const RespGetChunkMessage&) = delete;
    RespGetChunkMessage(RespGetChunkMessage&&) = delete;
    RespGetChunkMessage& operator=(const RespGetChunkMessage&) = delete;
    RespGetChunkMessage& operator=(RespGetChunkMessage&&) = delete;
    RespGetChunkMessage()
    {

    }

    ~RespGetChunkMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_GET_CHUNK_MESSAGE_HPP_
