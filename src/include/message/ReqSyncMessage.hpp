#ifndef SPARKLE_MESSAGE_REQUEST_SYNCHRONIZE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_SYNCHRONIZE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqSyncMessage : public Message {

public:
    uint64_t timestamp;
    uint32_t data_length;
    uint8_t* data;
    /* TODO: add other header or data segments */

public:
    ReqSyncMessage(const ReqSyncMessage&) = delete;
    ReqSyncMessage(ReqSyncMessage&&) = delete;
    ReqSyncMessage& operator=(const ReqSyncMessage&) = delete;
    ReqSyncMessage& operator=(ReqSyncMessage&&) = delete;
    ReqSyncMessage() :
        timestamp(0),
        data_length(0),
        data(nullptr)
    {

    }

    ~ReqSyncMessage()
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

#endif // SPARKLE_MESSAGE_REQUEST_SYNCHRONIZE_MESSAGE_HPP_
