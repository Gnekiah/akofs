#ifndef SPARKLE_MESSAGE_RESPONSE_COMMAND_REBOOT_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_COMMAND_REBOOT_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespCmdRebootMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    RespCmdRebootMessage(const RespCmdRebootMessage&) = delete;
    RespCmdRebootMessage(RespCmdRebootMessage&&) = delete;
    RespCmdRebootMessage& operator=(const RespCmdRebootMessage&) = delete;
    RespCmdRebootMessage& operator=(RespCmdRebootMessage&&) = delete;
    RespCmdRebootMessage()
    {

    }

    ~RespCmdRebootMessage()
    {

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

#endif // SPARKLE_MESSAGE_RESPONSE_COMMAND_REBOOT_MESSAGE_HPP_
