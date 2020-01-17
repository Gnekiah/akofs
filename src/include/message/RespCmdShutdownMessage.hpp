#ifndef SPARKLE_MESSAGE_RESPONSE_COMMAND_SHUTDOWN_MESSAGE_HPP_
#define SPARKLE_MESSAGE_RESPONSE_COMMAND_SHUTDOWN_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class RespCmdShutdownMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    RespCmdShutdownMessage(const RespCmdShutdownMessage&) = delete;
    RespCmdShutdownMessage(RespCmdShutdownMessage&&) = delete;
    RespCmdShutdownMessage& operator=(const RespCmdShutdownMessage&) = delete;
    RespCmdShutdownMessage& operator=(RespCmdShutdownMessage&&) = delete;
    RespCmdShutdownMessage()
    {

    }

    ~RespCmdShutdownMessage()
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

#endif // SPARKLE_MESSAGE_RESPONSE_COMMAND_SHUTDOWN_MESSAGE_HPP_
