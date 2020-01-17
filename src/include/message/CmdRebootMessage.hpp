#ifndef SPARKLE_MESSAGE_COMMAND_REBOOT_MESSAGE_HPP_
#define SPARKLE_MESSAGE_COMMAND_REBOOT_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class CmdRebootMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    CmdRebootMessage(const CmdRebootMessage&) = delete;
    CmdRebootMessage(CmdRebootMessage&&) = delete;
    CmdRebootMessage& operator=(const CmdRebootMessage&) = delete;
    CmdRebootMessage& operator=(CmdRebootMessage&&) = delete;
    CmdRebootMessage()
    {

    }

    ~CmdRebootMessage()
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

#endif // SPARKLE_MESSAGE_COMMAND_REBOOT_MESSAGE_HPP_
