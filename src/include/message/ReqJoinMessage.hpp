#ifndef SPARKLE_MESSAGE_REQUEST_JOIN_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_JOIN_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqJoinMessage : public Message {

public:
    uint64_t cluster_id;
    uint64_t group_id;
    uint16_t challenge_resp_offset;
    uint16_t challenge_resp_length;
    uint8_t* challenge_resp;
    /* TODO: add other header or data segments */

public:
    ReqJoinMessage(const ReqJoinMessage&) = delete;
    ReqJoinMessage(ReqJoinMessage&&) = delete;
    ReqJoinMessage& operator=(const ReqJoinMessage&) = delete;
    ReqJoinMessage& operator=(ReqJoinMessage&&) = delete;
    ReqJoinMessage() :
        cluster_id(0),
        group_id(0),
        challenge_resp_offset(0),
        challenge_resp_length(0),
        challenge_resp(nullptr)
    {

    }

    ~ReqJoinMessage()
    {
        delete challenge_resp;
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

#endif // SPARKLE_MESSAGE_REQUEST_JOIN_MESSAGE_HPP_
