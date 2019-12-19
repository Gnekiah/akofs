/*
 * Circular Buffer Template.
 *
 * When declaring your buffer you should specify the data type it must handle 
 * and the buffer capacity: those two parameters will influence the memory 
 * consumed by the buffer.
 *
 *
 * Copyright (C) 2019 CQU STARLab. All rights reserved.
 * Author: Xxiong <xxiong@cqu.edu.cn>
 */

#ifndef SPARKLE_CIRCULAR_BUFFER_HPP_
#define SPARKLE_CIRCULAR_BUFFER_HPP_

#include <stdint.h>
#include <stddef.h>
#include <spk_compat.h>

namespace spk {
    template<bool __FITS8, bool __FITS16> struct __CIRC_BUFF_INDEX {
        using Type = uint32_t;
    };
    template<> struct __CIRC_BUFF_INDEX<false, true> { using Type = uint16_t; };
    template<> struct __CIRC_BUFF_INDEX<true, true> { using Type = uint8_t; };

    template<typename T, size_t S, typename IT = typename
        spk::__CIRC_BUFF_INDEX<(S <= UINT8_MAX), (S <= UINT16_MAX)>::Type>
        class CircularBuffer {
        public:
            // The buffer capacity: read only as it cannot ever change.
#if __GNUC__ > 7 || _MSC_VER >= 1910
            /* new feature after C++17 */
            static constexpr IT capacity = static_cast<IT>(S);
            static constexpr T type = static_cast<T>(T());
#else
            size_t capacity = S;
            T type = { 0 };
#endif 
            // Aliases the index type, can be used to obtain the right index type
            // with `decltype(buffer)::index_t`.
            using index_t = IT;

            constexpr CircularBuffer();
            // Disables copy constructor
            CircularBuffer(const CircularBuffer&) = delete;
            CircularBuffer(CircularBuffer&&) = delete;

            // Disables assignment operator
            CircularBuffer& operator=(const CircularBuffer&) = delete;
            CircularBuffer& operator=(CircularBuffer&&) = delete;

            // Adds an element to the beginning of buffer: the operation returns 
            // `false` if the addition caused overwriting an existing element.
            bool PushFront(T value);

            // Adds an element to the end of buffer: the operation returns `false`
            // if the addition caused overwriting an existing element.
            bool PushBack(T value);

            // Removes an element from the beginning of the buffer.
            // *WARNING* calling this operation on an empty buffer has an unpredictable behaviour.
            T PopFront();

            // Removes an element from the end of the buffer.
            // *WARNING* calling this operation on an empty buffer has an unpredictable behaviour.
            T PopBack();

            // Returns the element at the beginning of the buffer.
            T inline first() const;

            // Returns the element at the end of the buffer.
            T inline last() const;

            // Array-like access to buffer
            T operator [] (IT index) const;

            CircularBuffer& operator <<(const T& str);
            CircularBuffer& operator >>(T& value);

            // Returns how many elements are actually stored in the buffer.
            IT inline size() const;

            // Returns how many elements can be safely pushed into the buffer.
            IT inline available() const;

            // Returns `true` if no elements can be removed from the buffer.
            bool inline isEmpty() const;

            // Returns `true` if no elements can be added to the buffer without 
            // overwriting existing elements.
            bool inline isFull() const;

            // Returns `true` if buffer could be coverable while head is ahead of tail
            bool inline coverable() const;

            // Set coverable, default is false
            void inline SetCoverable(bool b);

            // Resets the buffer to a clean status, making all buffer positions available.
            void inline Clear();

        private:
            T buffer_[S];
            T* volatile head_;
            T* volatile tail_;
            IT volatile count_;
            bool volatile coverable_;
    };

    template<typename T, size_t S, typename IT>
    constexpr CircularBuffer<T, S, IT>::CircularBuffer() :
        head_(buffer_), tail_(buffer_), count_(0), coverable_(false) {
    }

    template<typename T, size_t S, typename IT>
    bool CircularBuffer<T, S, IT>::PushFront(T value) {
        if (unlikely(count_ == capacity && coverable_ == false)) {
            return false;
        }
        head_ = unlikely(head_ == buffer_) ? buffer_ + capacity - 1 : head_ - 1;
        *head_ = value;

        if (unlikely(count_ == capacity)) {
            tail_ = unlikely(tail_ == buffer_) ? buffer_ + capacity - 1 : tail_ - 1;
            return false;
        }
        count_++;
        return true;
    }

    template<typename T, size_t S, typename IT>
    bool CircularBuffer<T, S, IT>::PushBack(T value) {
        if (unlikely(count_ == capacity && coverable_ == false)) {
            return false;
        }
        *tail_ = value;
        if (unlikely(++tail_ == buffer_ + capacity)) {
            tail_ = buffer_;
        }

        if (unlikely(count_ == capacity)) {
            if (unlikely(++head_ == buffer_ + capacity)) {
                head_ = buffer_;
            }
            return false;
        }
        count_++;
        return true;
    }

    template<typename T, size_t S, typename IT>
    T CircularBuffer<T, S, IT>::PopFront() {
        if (unlikely(count_ == 0)) {
            return type;
        }
        T result = *head_;
        head_ = unlikely(head_ + 1 == buffer_ + capacity) ? buffer_ : head_ + 1;
        count_--;
        return result;
    }

    template<typename T, size_t S, typename IT>
    T CircularBuffer<T, S, IT>::PopBack() {
        if (unlikely(count_ == 0)) {
            return type;
        }
        tail_ = unlikely(tail_ == buffer_) ? buffer_ + capacity - 1 : tail_ - 1;
        T result = *tail_;
        count_--;
        return result;
    }

    template<typename T, size_t S, typename IT>
    T inline CircularBuffer<T, S, IT>::first() const {
        if (unlikely(count_ == 0)) {
            return type;
        }
        return *head_;
    }

    template<typename T, size_t S, typename IT>
    T inline CircularBuffer<T, S, IT>::last() const {
        if (unlikely(count_ == 0)) {
            return type;
        }
        return *(unlikely(tail_ == buffer_) ? buffer_ + capacity - 1 : tail_ - 1);
    }

    template<typename T, size_t S, typename IT>
    T CircularBuffer<T, S, IT>::operator [](IT index) const {
        return *(buffer_ + ((head_ - buffer_ + index) % capacity));
    }

    template<typename T, size_t S, typename IT>
    CircularBuffer<T, S, IT>& CircularBuffer<T, S, IT>::operator <<(const T& str) {
        PushBack(str);
        return *this;
    }

    template<typename T, size_t S, typename IT>
    CircularBuffer<T, S, IT>& CircularBuffer<T, S, IT>::operator >>(T& value) {
        value = PopFront();
        return *this;
    }

    template<typename T, size_t S, typename IT>
    IT inline CircularBuffer<T, S, IT>::size() const {
        return count_;
    }

    template<typename T, size_t S, typename IT>
    IT inline CircularBuffer<T, S, IT>::available() const {
        return capacity - count_;
    }

    template<typename T, size_t S, typename IT>
    bool inline CircularBuffer<T, S, IT>::isEmpty() const {
        return count_ == 0;
    }

    template<typename T, size_t S, typename IT>
    bool inline CircularBuffer<T, S, IT>::isFull() const {
        return count_ == capacity;
    }

    template<typename T, size_t S, typename IT>
    bool inline CircularBuffer<T, S, IT>::coverable() const {
        return coverable_;
    }

    template<typename T, size_t S, typename IT>
    void inline CircularBuffer<T, S, IT>::SetCoverable(bool b) {
        coverable_ = b;
    }

    template<typename T, size_t S, typename IT>
    void inline CircularBuffer<T, S, IT>::Clear() {
        head_ = tail_ = buffer_;
        count_ = 0;
    }
}

#endif // SPARKLE_CIRCULAR_BUFFER_HPP_
