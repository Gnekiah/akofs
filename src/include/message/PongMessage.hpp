#ifndef SPARKLE_MESSAGE_PONG_MESSAGE_HPP_
#define SPARKLE_MESSAGE_PONG_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class PongMessage : public Message {

public:
    uint64_t timestamp;
    /* TODO: add other header or data segments */

public:
    PongMessage(const PongMessage&) = delete;
    PongMessage(PongMessage&&) = delete;
    PongMessage& operator=(const PongMessage&) = delete;
    PongMessage& operator=(PongMessage&&) = delete;
    PongMessage() :
        timestamp(0)
    {

    }

    ~PongMessage()
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

#endif // SPARKLE_MESSAGE_PONG_MESSAGE_HPP_
