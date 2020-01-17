#ifndef SPARKLE_MESSAGE_PING_MESSAGE_HPP_
#define SPARKLE_MESSAGE_PING_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class PingMessage : public Message {

public:
    uint64_t timestamp;
    /* TODO: add other header or data segments */

public:
    PingMessage(const PingMessage&) = delete;
    PingMessage(PingMessage&&) = delete;
    PingMessage& operator=(const PingMessage&) = delete;
    PingMessage& operator=(PingMessage&&) = delete;
    PingMessage() :
        timestamp(0)
    {

    }

    ~PingMessage()
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

#endif // SPARKLE_MESSAGE_PING_MESSAGE_HPP_