#ifndef SPARKLE_MESSAGE_RESPONSE_CONSISTENCY_CHECK_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_CONSISTENCY_CHECK_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespConsisCheckMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    RespConsisCheckMessage(const RespConsisCheckMessage&) = delete;
    RespConsisCheckMessage(RespConsisCheckMessage&&) = delete;
    RespConsisCheckMessage& operator=(const RespConsisCheckMessage&) = delete;
    RespConsisCheckMessage& operator=(RespConsisCheckMessage&&) = delete;
    RespConsisCheckMessage()
    {

    }

    ~RespConsisCheckMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_CONSISTENCY_CHECK_MESSAGE_HPP_
