#ifndef SPARKLE_MESSAGE_REQUEST_GET_CHUNK_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_GET_CHUNK_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqGetChunkMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    ReqGetChunkMessage(const ReqGetChunkMessage&) = delete;
    ReqGetChunkMessage(ReqGetChunkMessage&&) = delete;
    ReqGetChunkMessage& operator=(const ReqGetChunkMessage&) = delete;
    ReqGetChunkMessage& operator=(ReqGetChunkMessage&&) = delete;
    ReqGetChunkMessage()
    {

    }

    ~ReqGetChunkMessage()
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

#endif // SPARKLE_MESSAGE_REQUEST_GET_CHUNK_MESSAGE_HPP_
