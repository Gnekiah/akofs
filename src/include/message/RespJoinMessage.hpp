#ifndef SPARKLE_MESSAGE_RESPONSE_JOIN_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_JOIN_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespJoinMessage : public Message {

public:
    uint64_t cluster_id;
    uint64_t group_id;
    uint16_t join_resp;
    uint32_t data_length;
    uint8_t* data;
    /* TODO: add other header or data segments */

public:
    RespJoinMessage(const RespJoinMessage&) = delete;
    RespJoinMessage(RespJoinMessage&&) = delete;
    RespJoinMessage& operator=(const RespJoinMessage&) = delete;
    RespJoinMessage& operator=(RespJoinMessage&&) = delete;
    RespJoinMessage() :
        cluster_id(0),
        group_id(0),
        join_resp(0),
        data_length(0),
        data(nullptr)
    {

    }

    ~RespJoinMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_JOIN_MESSAGE_HPP_
