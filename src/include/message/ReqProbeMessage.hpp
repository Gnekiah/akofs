#ifndef SPARKLE_MESSAGE_REQUEST_PROBE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_PROBE_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqProbeMessage : public Message {

public:
    uint64_t cluster_id;
    uint64_t group_id;
    uint64_t node_id;
    uint8_t  scy_type;
    uint16_t pubkey_length;
    uint8_t* pubkey;
    /* TODO: add other header or data segments */

public:
    ReqProbeMessage(const ReqProbeMessage&) = delete;
    ReqProbeMessage(ReqProbeMessage&&) = delete;
    ReqProbeMessage& operator=(const ReqProbeMessage&) = delete;
    ReqProbeMessage& operator=(ReqProbeMessage&&) = delete;
    ReqProbeMessage() :
        cluster_id(0),
        group_id(0),
        node_id(0),
        scy_type(0),
        pubkey_length(0),
        pubkey(nullptr)
    {

    }

    ~ReqProbeMessage()
    {
        delete pubkey;
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

#endif // SPARKLE_MESSAGE_REQUEST_PROBE_MESSAGE_HPP_
