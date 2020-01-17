#ifndef SPARKLE_MESSAGE_RESPONSE_SUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_SUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespSubscribeSyncMessage : public Message {

public:
    uint16_t subscribe_sync_resp;
    uint32_t data_length;
    uint8_t* data;

    /* TODO: add other header or data segments */

public:
    RespSubscribeSyncMessage(const RespSubscribeSyncMessage&) = delete;
    RespSubscribeSyncMessage(RespSubscribeSyncMessage&&) = delete;
    RespSubscribeSyncMessage& operator=(const RespSubscribeSyncMessage&) = delete;
    RespSubscribeSyncMessage& operator=(RespSubscribeSyncMessage&&) = delete;
    RespSubscribeSyncMessage() :
        subscribe_sync_resp(0),
        data_length(0),
        data(nullptr)
    {

    }

    ~RespSubscribeSyncMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_SUBSCRIBE_SYNCHRONIZE_MESSAGE_HPP_

