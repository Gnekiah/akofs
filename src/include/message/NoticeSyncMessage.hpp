#ifndef SPARKLE_MESSAGE_NOTICE_SYNCHRONIZE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_NOTICE_SYNCHRONIZE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class NoticeSyncMessage : public Message {

public:
    uint16_t sync_data_offset;
    uint64_t sync_data_length;
    uint8_t* sync_data;

    /* TODO: add other header or data segments */

public:
    NoticeSyncMessage(const NoticeSyncMessage&) = delete;
    NoticeSyncMessage(NoticeSyncMessage&&) = delete;
    NoticeSyncMessage& operator=(const NoticeSyncMessage&) = delete;
    NoticeSyncMessage& operator=(NoticeSyncMessage&&) = delete;
    NoticeSyncMessage() :
        sync_data_offset(0),
        sync_data_length(0),
        sync_data(nullptr)
    {

    }

    ~NoticeSyncMessage()
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

#endif // SPARKLE_MESSAGE_NOTICE_SYNCHRONIZE_MESSAGE_HPP_
