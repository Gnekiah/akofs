#ifndef SPARKLE_MESSAGE_REQUEST_PUT_METADATA_MESSAGE_HPP_
#define SPARKLE_MESSAGE_REQUEST_PUT_METADATA_MESSAGE_HPP_

#include "Message.hpp"
#include <cstdint>
#include <spk_logger.h>

class ReqPutMetadataMessage : public Message {

public:
    /* TODO: add other header or data segments */

public:
    ReqPutMetadataMessage(const ReqPutMetadataMessage&) = delete;
    ReqPutMetadataMessage(ReqPutMetadataMessage&&) = delete;
    ReqPutMetadataMessage& operator=(const ReqPutMetadataMessage&) = delete;
    ReqPutMetadataMessage& operator=(ReqPutMetadataMessage&&) = delete;
    ReqPutMetadataMessage()
    {

    }

    ~ReqPutMetadataMessage()
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

#endif // SPARKLE_MESSAGE_REQUEST_PUT_METADATA_MESSAGE_HPP_
