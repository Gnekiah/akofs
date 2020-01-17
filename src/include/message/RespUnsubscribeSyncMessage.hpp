#ifndef SPARKLE_MESSAGE_RESPONSE_UNSUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_UNSUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespUnsubscribeSyncMessage : public Message {

public:
    uint16_t unsub_sync_resp;
    uint64_t timestamp;
    uint32_t data_length;
    uint8_t* data;

    /* TODO: add other header or data segments */

public:
    RespUnsubscribeSyncMessage(const RespUnsubscribeSyncMessage&) = delete;
    RespUnsubscribeSyncMessage(RespUnsubscribeSyncMessage&&) = delete;
    RespUnsubscribeSyncMessage& operator=(const RespUnsubscribeSyncMessage&) = delete;
    RespUnsubscribeSyncMessage& operator=(RespUnsubscribeSyncMessage&&) = delete;
    RespUnsubscribeSyncMessage() :
        unsub_sync_resp(0),
        timestamp(0),
        data_length(0),
        data(nullptr)
    {

    }

    ~RespUnsubscribeSyncMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_UNSUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_
