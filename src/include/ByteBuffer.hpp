/*
 * ByteBuffer in C++.
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef AKOFS_BYTE_BUFFER_H_
#define AKOFS_BYTE_BUFFER_H_

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <memory>

/* FUKOFF!!! `__cplusplus` in MSVC IS 199711L !!! */
#if defined(__GNUC__) && (__cplusplus <= 201103L)
namespace std {
    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... params) {
        return unique_ptr<T>(new T(forward<Args>(params)...));
    }
}
#endif

namespace ako {

    class ByteBuffer {
    public:
        // ByteBuffer constructor, Reserves specified size in internal vector
        // @param size Size (in bytes) of space to preallocate internally. 
        // Default is set in `ako_byte_buffer_default_size`
        ByteBuffer();

        // ByteBuffer constructor, Reserves specified size in internal vector
        // @param size Size (in bytes) of space to preallocate internally.
        ByteBuffer(uint64_t size);

        // ByteBuffer constructor, Consume an entire uint8_t array of length len in the ByteBuffer
        // @param arr uint8_t array of data (should be of length len)
        // @param size Size of space to allocate
        ByteBuffer(uint8_t* buf, uint64_t size);

        // Return a new instance of a ByteBuffer with the exact same contents and the
        // same state (rpos, wpos)
        ByteBuffer(const ByteBuffer& bbuf);

        ~ByteBuffer() = default;

        // Bytes Remaining
        // Returns the number of bytes from the current read position till the end of the buffer
        // @return Number of bytes from rpos to the end (size())
        uint64_t BytesRemaining() const { return write_pos_ - read_pos_; }

        // Clears out all data from the internal vector (original preallocated size remains)
        // resets the positions to 0
        void Clear();

        // Allocate an exact copy of the ByteBuffer on the heap and return a pointer
        // @return A pointer to the newly cloned ByteBuffer. NULL if no more memory available
        std::unique_ptr<ByteBuffer> Clone();

        // Equals, test for data equivilancy
        // Compare this ByteBuffer to another by looking at each byte in the internal buffers
        // and making sure they are the same
        // @param other A pointer to a ByteBuffer to compare to this one
        // @return True if the internal buffers match. False if otherwise
        bool Equals(const ByteBuffer& other);

        // Reallocates memory for the internal buffer of size newSize. Read and write 
        // positions will also be reset
        // @param newSize The amount of memory to allocate
        void Resize(uint64_t newSize);
        
        // Basic Searching (Linear)
        template<typename T>
        int64_t Find(T key, uint64_t start = 0);

        // Replace occurance of a particular uint8_t, key, with the uint8_t rep
        // @param key uint8_t to find for replacement
        // @param rep uint8_t to replace the found key with
        // @param start Index to start from. By default, start is 0
        // @param firstOccuranceOnly If true, only replace the first occurance of the key.
        // If false, replace all occurances. False by default
        void Replace(uint8_t key, uint8_t rep, uint64_t start = 0, bool firstOccuranceOnly = false);

        // Relative peek. Reads and returns the next uint8_t in the buffer from the current
        // position but does not increment the read position
        uint8_t Peek() const { return _read<uint8_t>(read_pos_); }

        template<typename T>
        T Get() const { return _read<T>(); }
        template<typename T>
        T Get(uint64_t index) const { return _read<T>(index); }

        void GetBytes(uint8_t* buf, uint64_t len, bool moveReadPos=true) const;
        void GetBytesFrom(uint8_t* buf, uint64_t len, uint64_t index) const;

        template<typename T>
        void Put(T t) { _write<T>(t); }
        template<typename T>
        void Put(T t, uint64_t index) { _write<T>(t, index); }

        void PutByteBuffer(ByteBuffer& src);
        void PutBytes(uint8_t* b, uint64_t len);
        void PutBytes(uint8_t* b, uint64_t len, uint64_t index); 

        // Buffer Position Accessors & Mutators
        void set_read_pos(size_t r) { read_pos_ = r % size(); }
        size_t read_pos() const { return read_pos_; }
        void set_write_pos(size_t w) { write_pos_ = w % size(); }
        size_t write_pos() const { return write_pos_; }
        size_t size() const { return buffer_.size(); }
        size_t capacity() const { return buffer_.capacity(); }

        
        template<typename T>
        ByteBuffer& operator <<(const T& value);
        ByteBuffer& operator <<(const std::string& str);
        ByteBuffer& operator <<(const char* str);

        template<typename T>
        ByteBuffer& operator >>(T& value);
        //ByteBuffer& operator >>(std::string& str);
        //ByteBuffer& operator >>(char* str);
        
        uint8_t operator [](size_t pos) { return _read<uint8_t>(pos); }
        ByteBuffer& operator =(const ByteBuffer& bbuf);
        bool operator ==(const ByteBuffer& bbuf);

        template<typename... Args>
        ByteBuffer& Packed(Args ...args) { return _packed(args...); }
        template<typename... Args>
        ByteBuffer& Unpacked(Args& ...args) { return _unpacked(args...); }

    private:
        template<typename T>
        T _read() const;
        template<typename T>
        T _read(const uint64_t index) const;
        template<typename T>
        void _write(const T t);
        template<typename T>
        void _write(const T t, const uint64_t index);

        template<typename T>
        ByteBuffer& _packed(T& t);
        template<typename T, typename... Args>
        ByteBuffer& _packed(T& t, Args& ...args);
        template<typename T>
        ByteBuffer& _unpacked(T& t);
        template<typename T, typename... Args>
        ByteBuffer& _unpacked(T& t, Args& ...args);
        
    private:
        size_t write_pos_;
        mutable size_t read_pos_;
        std::vector<uint8_t> buffer_;
    };

    ByteBuffer::ByteBuffer() {
        // Default number of uint8_ts to allocate in the backing buffer if no size is provided
        static const uint64_t ako_byte_buffer_default_size = 4096;
        buffer_.reserve(ako_byte_buffer_default_size);
        Clear();
    }

    ByteBuffer::ByteBuffer(uint64_t size) {
        buffer_.reserve(size);
        Clear();
    }

    ByteBuffer::ByteBuffer(uint8_t* buf, uint64_t size) {
        // If the provided array is NULL, allocate a blank buffer of the provided size
        if (buf == NULL) {
            buffer_.reserve(size);
            Clear();
        }
        else { // Consume the provided array
            buffer_.reserve(size);
            Clear();
            PutBytes(buf, size);
        }
    }

    ByteBuffer::ByteBuffer(const ByteBuffer& bbuf) {
        buffer_.reserve(bbuf.size());
        Clear();
        for (uint64_t i = 0; i < bbuf.size(); i++) {
            Put(bbuf.Get<uint8_t>(i));
        }

        // Reset positions
        read_pos_ = 0;
        write_pos_ = 0;
    }

    void ByteBuffer::Clear() {
        read_pos_ = 0;
        write_pos_ = 0;
        buffer_.clear();
    }

    std::unique_ptr<ByteBuffer> ByteBuffer::Clone() {
        std::unique_ptr<ByteBuffer> ret = std::make_unique<ByteBuffer>(buffer_.size());
        
        // Copy data
        for (uint64_t i = 0; i < buffer_.size(); i++) {
            ret->Put(Get<uint8_t>(i));
        }

        // Reset positions
        ret->set_read_pos(0);
        ret->set_write_pos(0);

        return ret;
    }


    bool ByteBuffer::Equals(const ByteBuffer& other) {
        // If sizes aren't equal, they can't be equal
        if (size() != other.size())
            return false;

        // Compare byte by byte
        uint64_t len = size();
        for (uint64_t i = 0; i < len; i++) {
            if (Get<uint8_t>(i) != other.Get<uint8_t>(i))
                return false;
        }

        return true;
    }


    void ByteBuffer::Resize(uint64_t newSize) {
        buffer_.resize(newSize);
        read_pos_ = 0;
        write_pos_ = 0;
    }

    template<typename T>
    int64_t ByteBuffer::Find(T key, uint64_t start) {
        int64_t ret = -1;
        uint64_t len = buffer_.size();
        for (uint64_t i = start; i < len; i++) {
            T data = _read<T>(i);
            // Wasn't actually found, bounds of buffer were exceeded
            if ((key != 0) && (data == 0))
                break;

            // Key was found in array
            if (data == key) {
                ret = (int64_t)i;
                break;
            }
        }
        return ret;
    }

    void ByteBuffer::Replace(uint8_t key, uint8_t rep, uint64_t start, bool firstOccuranceOnly) {
        uint64_t len = buffer_.size();
        for (uint64_t i = start; i < len; i++) {
            uint8_t data = _read<uint8_t>(i);
            // Wasn't actually found, bounds of buffer were exceeded
            if ((key != 0) && (data == 0))
                break;

            // Key was found in array, perform replacement
            if (data == key) {
                buffer_[i] = rep;
                if (firstOccuranceOnly)
                    return;
            }
        }
    }

    void ByteBuffer::GetBytes(uint8_t* buf, uint64_t len, bool moveReadPos) const {
        for (uint64_t i = 0, index = read_pos_; i < len; i++, index++) {
            buf[i] = moveReadPos ? _read<uint8_t>() : _read<uint8_t>(index);
        }
    }

    void ByteBuffer::GetBytesFrom(uint8_t* buf, uint64_t len, uint64_t index) const {
        for (uint64_t i = 0; i < len; i++, index++) {
            buf[i] = _read<uint8_t>(index);
        }
    }

    void ByteBuffer::PutByteBuffer(ByteBuffer& src) {
        *this = src;
    }

    void ByteBuffer::PutBytes(uint8_t* b, uint64_t len) {
        // Insert the data one byte at a time into the internal buffer at position i+starting index
        for (uint64_t i = 0; i < len; i++)
            _write<uint8_t>(b[i]);
    }

    void ByteBuffer::PutBytes(uint8_t* b, uint64_t len, uint64_t index) {
        write_pos_ = index;

        // Insert the data one byte at a time into the internal buffer at position i+starting index
        for (uint64_t i = 0; i < len; i++)
            _write<uint8_t>(b[i]);
    }

    template<typename T>
    ByteBuffer& ByteBuffer::operator <<(const T& value) {
        _write(value);
        return *this;
    }

    ByteBuffer& ByteBuffer::operator <<(const std::string& str) {
        uint64_t len = str.length();
        for (int i = 0; i < len; i++)
            _write(str[i]);
        return *this;
    }

    ByteBuffer& ByteBuffer::operator <<(const char* str) {
        uint64_t len = strlen(str);
        for (int i=0; i < len; i++)
            _write(str[i]);
        return *this;
    }

    template<typename T>
    ByteBuffer& ByteBuffer::operator >>(T& value) {
        if (read_pos_ + sizeof(T) <= buffer_.size()) {
            memcpy(&value, buffer_.data() + read_pos_, sizeof(T));
            read_pos_ += sizeof(T);
        }
        return *this;
    }

    //ByteBuffer& ByteBuffer::operator >>(std::string& str) {
    //    uint64_t size = _read<uint64_t>();
    //    for (uint64_t i = 0; i < size; i++)
    //        str.push_back(_read<char>());
    //    return *this;
    //}

    //ByteBuffer& ByteBuffer::operator >>(char* str) {
    //    uint64_t size = _read<uint64_t>();
    //    for (uint64_t i = 0; i < size; i++)
    //        str[i] = _read<char>();
    //    return *this;
    //}

    ByteBuffer& ByteBuffer::operator =(const ByteBuffer& bbuf) {
        if (this == &bbuf)
            return *this;

        uint64_t buf_size = bbuf.size();
        Resize(buf_size);
        for (uint64_t i = 0; i < buf_size; i++) {
            Put(bbuf.Get<uint8_t>(i));
        }

        read_pos_ = 0;
        write_pos_ = 0;

        return *this;
    }

    bool ByteBuffer::operator ==(const ByteBuffer& bbuf) {
        return Equals(bbuf);
    }

    template<typename T>
    T ByteBuffer::_read() const {
        T data = _read<T>(read_pos_);
        read_pos_ += sizeof(T);
        return data;
    }

    template<typename T>
    T ByteBuffer::_read(uint64_t index) const {
        if (index + sizeof(T) <= buffer_.size())
            return *((T*)& buffer_[index]);
        return 0;
    }

    template<typename T>
    void ByteBuffer::_write(T data) {
        uint64_t s = sizeof(data);

        if (size() < (write_pos_ + s))
            buffer_.resize(write_pos_ + s);
        memcpy(&buffer_[write_pos_], (uint8_t*)& data, s);
        write_pos_ += s;
    }

    template<typename T>
    void ByteBuffer::_write(T data, uint64_t index) {
        if ((index + sizeof(data)) > size())
            return;
        memcpy(&buffer_[index], (uint8_t*)& data, sizeof(data));
        write_pos_ = index + sizeof(data);
    }

    template<typename T, typename... Args>
    ByteBuffer& ByteBuffer::_packed(T& t, Args& ...args) {
        *this << t;
        return _packed(args...);
    }

    template<typename T, typename... Args>
    ByteBuffer& ByteBuffer::_unpacked(T& t, Args& ...args) {
        *this >> t;
        return _unpacked(args...);
    }

    template<typename T>
    ByteBuffer& ByteBuffer::_packed(T& t) {
        *this << t;
        return *this;
    }

    template<typename T>
    ByteBuffer& ByteBuffer::_unpacked(T& t) {
        *this >> t;
        return *this;
    }

}

#endif // AKOFS_BYTE_BUFFER_H_
