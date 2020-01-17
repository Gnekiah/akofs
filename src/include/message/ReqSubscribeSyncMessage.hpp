#ifndef SPARKLE_MESSAGE_REQUEST_SUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_SUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqSubscribeSyncMessage : public Message {

public:
    uint16_t update_rate;
    uint16_t update_threshold;
    uint64_t timestamp;
    uint32_t data_length;
    uint8_t* data;

    /* TODO: add other header or data segments */

public:
    ReqSubscribeSyncMessage(const ReqSubscribeSyncMessage&) = delete;
    ReqSubscribeSyncMessage(ReqSubscribeSyncMessage&&) = delete;
    ReqSubscribeSyncMessage& operator=(const ReqSubscribeSyncMessage&) = delete;
    ReqSubscribeSyncMessage& operator=(ReqSubscribeSyncMessage&&) = delete;
    ReqSubscribeSyncMessage() :
        update_rate(0),
        update_threshold(0),
        timestamp(0),
        data_length(0),
        data(nullptr)
    {

    }

    ~ReqSubscribeSyncMessage()
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

#endif // SPARKLE_MESSAGE_REQUEST_SUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_
