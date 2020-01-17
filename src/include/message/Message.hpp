/*
 * Message Base.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_MESSAGE_MESSAGE_HPP_
#define SPARKLE_MESSAGE_MESSAGE_HPP_

#include <spk_errno.h>
#include <spk_logger.h>

class Message {

public:
    uint8_t* message_header_buff;
    uint64_t message_header_size;
    uint8_t* extra_header_buff;
    uint64_t extra_header_size;
    uint8_t* message_data_buff;
    uint64_t message_data_size;

public:
    Message(const Message&) = delete;
    Message(Message&&) = delete;
    Message& operator=(const Message&) = delete;
    Message& operator=(Message&&) = delete;
    Message() : 
        message_header_buff(nullptr),
        message_header_size(0),
        extra_header_buff(nullptr),
        extra_header_size(0),
        message_data_buff(nullptr),
        message_data_size(0)
    {
        _spiu_._spiu_buff_ = nullptr;
        _spiu_._spiu_size_ = 0;
    }
    ~Message() 
    {
        delete message_header_buff;
        delete extra_header_buff;
        delete message_data_buff;
        delete _spiu_._spiu_buff_;
    }

    virtual int Dump() = 0;
    virtual int Pack() = 0;

public:
    /* Sparkle Protocol Information Unit Header Segment */
    /* Controlled and Used by Sparkle Low-Level Service */
    /* DO NOT Modify the Segment on Top of Virtual Node */
    struct _spiu_ {
        uint8_t* _spiu_buff_;
        uint64_t _spiu_size_;

        uint16_t _message_code_;
        uint8_t _prot_ver_;
        uint8_t _flag_;
        uint16_t _msg_header_length_;
        uint16_t _data_offset_;
        uint32_t _data_length_;
        uint16_t _EHS_length_;
        uint16_t _reversed1_;
        uint64_t _conn_session_id_;
        uint64_t _message_sequence_;
        uint64_t _node_id_from_;
        uint64_t _node_id_to_;
    } _spiu_ = { 0 };

    int _dump_message(uint8_t* buff, uint64_t size)
    {
        int err = 0;

        _spiu_._spiu_buff_ = buff;
        _spiu_._spiu_size_ = size;
        if (size < 48 /*sizeof(struct _spiu_)*/ ) {
            spklog_warn(LOGGING_POSITION, "dump sparkle message failed.");
            return -SPKE_MESSAGE_DUMP_DATA_INSUFFICIENT;
        }
        
        /* spiu header dumping */
        _spiu_._message_code_ = (uint16_t)buff;
        buff += sizeof(_spiu_._message_code_);
        _spiu_._prot_ver_ = (uint8_t)buff;
        buff += sizeof(_spiu_._prot_ver_);
        _spiu_._flag_ = (uint8_t)buff;
        buff += sizeof(_spiu_._flag_);
        _spiu_._msg_header_length_ = (uint16_t)buff;
        buff += sizeof(_spiu_._msg_header_length_);
        _spiu_._data_offset_ = (uint16_t)buff;
        buff += sizeof(_spiu_._data_offset_);

        _spiu_._data_length_ = (uint32_t)buff;
        buff += sizeof(_spiu_._data_length_);
        _spiu_._EHS_length_ = (uint16_t)buff;
        buff += sizeof(_spiu_._EHS_length_);
        _spiu_._reversed1_ = (uint16_t)buff;
        buff += sizeof(_spiu_._reversed1_);

        _spiu_._conn_session_id_ = (uint64_t)buff;
        buff += sizeof(_spiu_._conn_session_id_);
        _spiu_._message_sequence_ = (uint64_t)buff;
        buff += sizeof(_spiu_._message_sequence_);

        _spiu_._node_id_from_ = (uint64_t)buff;
        buff += sizeof(_spiu_._node_id_from_);
        _spiu_._node_id_to_ = (uint64_t)buff;
        buff += sizeof(_spiu_._node_id_to_);

        /* message header dumping */
        if (size <= buff - _spiu_._spiu_buff_) {
            return -SPKE_MESSAGE_DUMP_DATA_INSUFFICIENT;
        }
        message_header_buff = buff;
        message_header_size = _spiu_._msg_header_length_;
        buff += message_header_size;

        /* extra header dumping */
        if (size <= buff - _spiu_._spiu_buff_) {
            return -SPKE_MESSAGE_DUMP_DATA_INSUFFICIENT;
        }
        if (_spiu_._EHS_length_ > 0) {
            extra_header_buff = buff;
            extra_header_size = _spiu_._EHS_length_;
            buff += extra_header_size;
        }
        
        /* message data dumping */
        if (size <= buff - _spiu_._spiu_buff_) {
            return -SPKE_MESSAGE_DUMP_DATA_INSUFFICIENT;
        }
        if (buff - _spiu_._spiu_buff_ != _spiu_._data_offset_) {
            return -SPKE_MESSAGE_DUMP_PACKAGE_ERROR;
        }
        message_data_buff = buff;
        message_data_size = _spiu_._data_length_;

        return 0;
    }

    int _pack_message()
    {
        _spiu_._spiu_size_ = 48 + message_header_size + extra_header_size
            + message_data_size;
        _spiu_._spiu_buff_ = new uint8_t[_spiu_._spiu_size_]();
        if (_spiu_._spiu_buff_ = nullptr) {
            return -SPKE_NOMEM;
        }

        /* TODO: Init _SPIU_ Header and Do Pack */
        _spiu_._message_code_ = 0;          /* TODO */
        _spiu_._prot_ver_ = 0;              /* TODO */
        _spiu_._flag_ = 0;                  /* TODO */
        _spiu_._msg_header_length_ = 0;     /* TODO */
        _spiu_._data_offset_ = 0;           /* TODO */
        _spiu_._data_length_ = 0;           /* TODO */
        _spiu_._EHS_length_ = 0;            /* TODO */
        _spiu_._reversed1_ = 0;             /* TODO */
        _spiu_._conn_session_id_ = 0;       /* TODO */
        _spiu_._message_sequence_ = 0;      /* TODO */
        _spiu_._node_id_from_ = 0;          /* TODO */
        _spiu_._node_id_to_ = 0;            /* TODO */


        uint8_t* buff_ptr = _spiu_._spiu_buff_;
        memcpy(buff_ptr, (void*)_spiu_._message_code_, sizeof(_spiu_._message_code_));
        buff_ptr += sizeof(_spiu_._message_code_);
        memcpy(buff_ptr, (void*)_spiu_._prot_ver_, sizeof(_spiu_._prot_ver_));
        buff_ptr += sizeof(_spiu_._prot_ver_);
        memcpy(buff_ptr, (void*)_spiu_._flag_, sizeof(_spiu_._flag_));
        buff_ptr += sizeof(_spiu_._flag_);

        memcpy(buff_ptr, (void*)_spiu_._msg_header_length_, sizeof(_spiu_._msg_header_length_));
        buff_ptr += sizeof(_spiu_._msg_header_length_);
        memcpy(buff_ptr, (void*)_spiu_._data_offset_, sizeof(_spiu_._data_offset_));
        buff_ptr += sizeof(_spiu_._data_offset_);
        memcpy(buff_ptr, (void*)_spiu_._data_length_, sizeof(_spiu_._data_length_));
        buff_ptr += sizeof(_spiu_._data_length_);
        
        memcpy(buff_ptr, (void*)_spiu_._EHS_length_, sizeof(_spiu_._EHS_length_));
        buff_ptr += sizeof(_spiu_._EHS_length_);
        memcpy(buff_ptr, (void*)_spiu_._reversed1_, sizeof(_spiu_._reversed1_));
        buff_ptr += sizeof(_spiu_._reversed1_);
        
        memcpy(buff_ptr, (void*)_spiu_._conn_session_id_, sizeof(_spiu_._conn_session_id_));
        buff_ptr += sizeof(_spiu_._conn_session_id_);
        memcpy(buff_ptr, (void*)_spiu_._message_sequence_, sizeof(_spiu_._message_sequence_));
        buff_ptr += sizeof(_spiu_._message_sequence_);

        memcpy(buff_ptr, (void*)_spiu_._node_id_from_, sizeof(_spiu_._node_id_from_));
        buff_ptr += sizeof(_spiu_._node_id_from_);
        memcpy(buff_ptr, (void*)_spiu_._node_id_to_, sizeof(_spiu_._node_id_to_));
        buff_ptr += sizeof(_spiu_._node_id_to_);

        memcpy(buff_ptr, (void*)message_header_buff, message_header_size);
        buff_ptr += message_header_size;
        memcpy(buff_ptr, (void*)extra_header_buff, extra_header_size);
        buff_ptr += extra_header_size;
        memcpy(buff_ptr, (void*)message_data_buff, message_data_size);
        buff_ptr += message_data_size;

        return 0;
    }
};

#endif // SPARKLE_MESSAGE_MESSAGE_HPP_