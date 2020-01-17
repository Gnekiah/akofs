#ifndef SPARKLE_MESSAGE_REQUEST_UNSUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_UNSUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqUnsubscribeSyncMessage : public Message {

public:
    uint32_t data_length;
    uint8_t* data;

    /* TODO: add other header or data segments */

public:
    ReqUnsubscribeSyncMessage(const ReqUnsubscribeSyncMessage&) = delete;
    ReqUnsubscribeSyncMessage(ReqUnsubscribeSyncMessage&&) = delete;
    ReqUnsubscribeSyncMessage& operator=(const ReqUnsubscribeSyncMessage&) = delete;
    ReqUnsubscribeSyncMessage& operator=(ReqUnsubscribeSyncMessage&&) = delete;
    ReqUnsubscribeSyncMessage() :
        data_length(0),
        data(nullptr)
    {

    }

    ~ReqUnsubscribeSyncMessage()
    {
        delete data;
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

#endif // SPARKLE_MESSAGE_REQUEST_UNSUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_
