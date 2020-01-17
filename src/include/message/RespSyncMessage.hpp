#ifndef SPARKLE_MESSAGE_RESPONSE_SYNCHRONIZE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_SYNCHRONIZE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespSyncMessage : public Message {

public:
    uint64_t timestamp;
    uint32_t sync_data_length;
    uint8_t* sync_data;
    /* TODO: add other header or data segments */

public:
    RespSyncMessage(const RespSyncMessage&) = delete;
    RespSyncMessage(RespSyncMessage&&) = delete;
    RespSyncMessage& operator=(const RespSyncMessage&) = delete;
    RespSyncMessage& operator=(RespSyncMessage&&) = delete;
    RespSyncMessage() :
        timestamp(0),
        sync_data_length(0),
        sync_data(nullptr)
    {

    }

    ~RespSyncMessage()
    {
        delete sync_data;
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

#endif // SPARKLE_MESSAGE_RESPONSE_SYNCHRONIZE_MESSAGE_HPP_
