#ifndef SPARKLE_MESSAGE_REQUEST_CONSISTENCY_CHECK_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_CONSISTENCY_CHECK_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqConsisCheckMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    ReqConsisCheckMessage(const ReqConsisCheckMessage&) = delete;
    ReqConsisCheckMessage(ReqConsisCheckMessage&&) = delete;
    ReqConsisCheckMessage& operator=(const ReqConsisCheckMessage&) = delete;
    ReqConsisCheckMessage& operator=(ReqConsisCheckMessage&&) = delete;
    ReqConsisCheckMessage()
    {

    }

    ~ReqConsisCheckMessage()
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

#endif // SPARKLE_MESSAGE_REQUEST_CONSISTENCY_CHECK_MESSAGE_HPP_
