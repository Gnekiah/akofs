#ifndef SPARKLE_MESSAGE_REQUEST_PUT_CHUNK_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_PUT_CHUNK_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqPutChunkMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    ReqPutChunkMessage(const ReqPutChunkMessage&) = delete;
    ReqPutChunkMessage(ReqPutChunkMessage&&) = delete;
    ReqPutChunkMessage& operator=(const ReqPutChunkMessage&) = delete;
    ReqPutChunkMessage& operator=(ReqPutChunkMessage&&) = delete;
    ReqPutChunkMessage()
    {

    }

    ~ReqPutChunkMessage()
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

#endif // SPARKLE_MESSAGE_REQUEST_PUT_CHUNK_MESSAGE_HPP_
