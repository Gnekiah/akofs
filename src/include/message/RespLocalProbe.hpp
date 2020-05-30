#ifndef SPARKLE_MESSAGE_COMMAND_SHUTDOWN_MESSAGE_HPP_
#define SPARKLE_MESSAGE_COMMAND_SHUTDOWN_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class CmdShutdownMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    CmdShutdownMessage(const CmdShutdownMessage&) = delete;
    CmdShutdownMessage(CmdShutdownMessage&&) = delete;
    CmdShutdownMessage& operator=(const CmdShutdownMessage&) = delete;
    CmdShutdownMessage& operator=(CmdShutdownMessage&&) = delete;
    CmdShutdownMessage()
    {

    }

    ~CmdShutdownMessage()
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

#endif // SPARKLE_MESSAGE_COMMAND_SHUTDOWN_MESSAGE_HPP_
