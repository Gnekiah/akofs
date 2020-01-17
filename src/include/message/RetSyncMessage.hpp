#ifndef SPARKLE_MESSAGE_RETURN_SYNCHRONIZE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RETURN_SYNCHRONIZE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RetSyncMessage : public Message {

public:
    uint16_t sync_data_offset;
    uint16_t sync_data_length;
    uint8_t* sync_data;

    /* TODO: add other header or data segments */

public:
    RetSyncMessage(const RetSyncMessage&) = delete;
    RetSyncMessage(RetSyncMessage&&) = delete;
    RetSyncMessage& operator=(const RetSyncMessage&) = delete;
    RetSyncMessage& operator=(RetSyncMessage&&) = delete;
    RetSyncMessage() :
        sync_data_offset(0),
        sync_data_length(0),
        sync_data(nullptr)
    {

    }

    ~RetSyncMessage()
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

#endif // SPARKLE_MESSAGE_RETURN_SYNCHRONIZE_MESSAGE_HPP_
