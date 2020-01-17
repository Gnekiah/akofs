#ifndef SPARKLE_MESSAGE_RESPONSE_PROBE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_PROBE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespProbeMessage : public Message {

public:
    uint64_t cluster_id;
    uint64_t group_id;
    uint8_t  scy_type;
    uint16_t pubkey_length;
    uint16_t challenge_offset;
    uint16_t challenge_length;
    uint8_t* pubkey;
    uint8_t* challenge;
    /* TODO: add other header or data segments */

public:
    RespProbeMessage(const RespProbeMessage&) = delete;
    RespProbeMessage(RespProbeMessage&&) = delete;
    RespProbeMessage& operator=(const RespProbeMessage&) = delete;
    RespProbeMessage& operator=(RespProbeMessage&&) = delete;
    RespProbeMessage() :
        cluster_id(0),
        group_id(0),
        scy_type(0),
        pubkey_length(0),
        challenge_offset(0),
        challenge_length(0),
        pubkey(nullptr),
        challenge(nullptr)
    {

    }

    ~RespProbeMessage()
    {
        delete pubkey;
        delete challenge;
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

#endif // SPARKLE_MESSAGE_RESPONSE_PROBE_MESSAGE_HPP_
