#ifndef SPARKLE_TEST_CIRCULAR_BUFFER_HPP_
#define SPARKLE_TEST_CIRCULAR_BUFFER_HPP_

#include <gtest/gtest.h>
#include <CircularBuffer.hpp>
#include <sstream>
#include <spk_compat.h>

#define PRINT_BUFFER(buffer, ss) do { \
    ss.str(""); \
    if (buffer.isEmpty()) { ss << "empty"; } \
    else { ss << "["; \
        for (decltype(buffer)::index_t i = 0; i < buffer.size() - 1; i++) { \
            ss << buffer[i] << ","; } \
        ss << buffer[buffer.size() - 1] << "] (" << (int)buffer.size() << "/" \
            << (int)buffer.available(); \
        if (buffer.isFull()) { ss << " full"; } \
        ss << ")"; \
    } \
} while(0)

typedef struct Item1Field {
    uint8_t v1;
    uint64_t v2;
} Item1Field;

typedef SPK_PACKED(struct Item2Field {
    uint8_t v1;
    uint64_t v2;
}) Item2Field;

#define STACK_REGION_MAY_OFFSET  int64_t(8 * 1024)

TEST(test_CircularBuffer, case_memory_region_heap_and_stack_test_1) {
    int stack_from = 0;
    spk::CircularBuffer<Item1Field, 10> item1Buffer;
    spk::CircularBuffer<Item1Field, 10> * item2Buffer = new spk::CircularBuffer<Item1Field, 10>();
    int stack_to = 0;

    // while on the working device the msvc's stack address is lower than heap
    // EXPECT_LT((uint64_t)&stack_to, (uint64_t)item2Buffer);
    EXPECT_GT(STACK_REGION_MAY_OFFSET, llabs((int64_t)&item1Buffer - (int64_t)&stack_to));
    EXPECT_LT(STACK_REGION_MAY_OFFSET, llabs((int64_t)item2Buffer - (int64_t)&stack_to));
    EXPECT_EQ(sizeof(spk::CircularBuffer<Item1Field, 10>*), sizeof(item2Buffer));

    uint64_t mem_region_upper = sizeof(Item1Field) * 11 + sizeof(char*) * 2 + 8 + 8 + 8;
    uint64_t mem_region_lower = sizeof(Item1Field) * 10 + sizeof(char*) * 2 + 2;
    EXPECT_LE(mem_region_lower, sizeof(item1Buffer));
    EXPECT_GE(mem_region_upper, sizeof(item1Buffer));

    uint64_t buff_between_offset = (uint64_t)item2Buffer - (uint64_t)& item1Buffer;
    EXPECT_LT(STACK_REGION_MAY_OFFSET, buff_between_offset);

    delete item2Buffer;
}

TEST(test_CircularBuffer, case_static_constexpr_static_cast_test_1) {
    spk::CircularBuffer<char, 10> char1Buffer;
    spk::CircularBuffer<char, 10> * char2Buffer = new spk::CircularBuffer<char, 10>();
    spk::CircularBuffer<Item1Field, 10> item1Buffer;
    spk::CircularBuffer<Item1Field, 10> * item2Buffer = new spk::CircularBuffer<Item1Field, 10>();

    spk::CircularBuffer<char, 100> char3Buffer;
    spk::CircularBuffer<char, 101> * char4Buffer = new spk::CircularBuffer<char, 101>();
    spk::CircularBuffer<Item1Field, 102> item3Buffer;
    spk::CircularBuffer<Item1Field, 103> * item4Buffer = new spk::CircularBuffer<Item1Field, 103>();

#if _MSC_VER >= 1910 || __GNUC__ >= 7
    /* static constexpr capacity check */
    EXPECT_EQ(&char1Buffer.capacity, &char2Buffer->capacity);
    EXPECT_NE(&char1Buffer.capacity, &item1Buffer.capacity);
    EXPECT_NE(&char1Buffer.capacity, &item2Buffer->capacity);
    EXPECT_NE(&char2Buffer->capacity, &item1Buffer.capacity);
    EXPECT_NE(&char2Buffer->capacity, &item2Buffer->capacity);
    EXPECT_EQ(&item1Buffer.capacity, &item2Buffer->capacity);

    EXPECT_NE(&char1Buffer.capacity, &char3Buffer.capacity);
    EXPECT_NE(&char1Buffer.capacity, &char4Buffer->capacity);
    EXPECT_NE(&item1Buffer.capacity, &item3Buffer.capacity);
    EXPECT_NE(&item1Buffer.capacity, &item4Buffer->capacity);

    EXPECT_NE(&char3Buffer.capacity, &char4Buffer->capacity);
    EXPECT_NE(&char3Buffer.capacity, &item3Buffer.capacity);
    EXPECT_NE(&char3Buffer.capacity, &item4Buffer->capacity);
    EXPECT_NE(&char4Buffer->capacity, &item3Buffer.capacity);
    EXPECT_NE(&char4Buffer->capacity, &item4Buffer->capacity);
    EXPECT_NE(&item3Buffer.capacity, &item4Buffer->capacity);

    /* static constexpr type check */
    EXPECT_EQ(&char1Buffer.type, &char2Buffer->type);
    EXPECT_NE((uint64_t)&char1Buffer.type, (uint64_t)&item1Buffer.type);
    EXPECT_NE((uint64_t)&char1Buffer.type, (uint64_t)&item2Buffer->type);
    EXPECT_NE((uint64_t)&char2Buffer->type, (uint64_t)&item1Buffer.type);
    EXPECT_NE((uint64_t)&char2Buffer->type, (uint64_t)&item2Buffer->type);
    EXPECT_EQ(&item1Buffer.type, &item2Buffer->type);

    EXPECT_NE(&char1Buffer.type, &char3Buffer.type);
    EXPECT_NE(&char1Buffer.type, &char4Buffer->type);
    EXPECT_NE(&item1Buffer.type, &item3Buffer.type);
    EXPECT_NE(&item1Buffer.type, &item4Buffer->type);

    EXPECT_NE(&char3Buffer.type, &char4Buffer->type);
    EXPECT_NE((uint64_t)&char3Buffer.type, (uint64_t)&item3Buffer.type);
    EXPECT_NE((uint64_t)&char3Buffer.type, (uint64_t)&item4Buffer->type);
    EXPECT_NE((uint64_t)&char4Buffer->type, (uint64_t)&item3Buffer.type);
    EXPECT_NE((uint64_t)&char4Buffer->type, (uint64_t)&item4Buffer->type);
    EXPECT_NE(&item3Buffer.type, &item4Buffer->type);
#endif

    EXPECT_EQ(0, char2Buffer->type);
    EXPECT_EQ(0, item4Buffer->type.v1);
    EXPECT_EQ(0, item4Buffer->type.v2);

    delete char2Buffer;
    delete item2Buffer;
    delete char4Buffer;
    delete item4Buffer;
}

TEST(test_CircularBuffer, case_memory_region_typeof_struct_packed_test_1) {
    EXPECT_LT(9, sizeof(Item1Field));
    EXPECT_EQ(9, sizeof(Item2Field));
}

TEST(test_CircularBuffer, case_test_1) {
    int ret;
    char curr;
    spk::CircularBuffer<char, 10> buffer;
    std::stringstream ss;

    buffer.PushBack('A');
    buffer.PushBack('B');
    buffer.PushBack('C');
    buffer.PushBack('D');
    buffer.PushBack('E');
    buffer.PushBack('F');
    buffer.PushBack('G');
    buffer.PushBack('H');
    buffer.PushBack('I');
    ret = buffer.PushBack('J');
    EXPECT_EQ(true, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[A,B,C,D,E,F,G,H,I,J] (10/0 full)", ss.str().c_str());
    
    curr = buffer.first();
    EXPECT_EQ('A', curr);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[A,B,C,D,E,F,G,H,I,J] (10/0 full)", ss.str().c_str());

    curr = buffer.last();
    EXPECT_EQ('J', curr);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[A,B,C,D,E,F,G,H,I,J] (10/0 full)", ss.str().c_str());

    ret = buffer.PushFront('X');
    EXPECT_EQ(false, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[A,B,C,D,E,F,G,H,I,J] (10/0 full)", ss.str().c_str());

    ret = buffer.PushBack('Z');
    EXPECT_EQ(false, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[A,B,C,D,E,F,G,H,I,J] (10/0 full)", ss.str().c_str());
    EXPECT_EQ(10, buffer.size());

    curr = buffer.PopBack();
    EXPECT_EQ('J', curr);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[A,B,C,D,E,F,G,H,I] (9/1)", ss.str().c_str());
    EXPECT_EQ(9, buffer.size());

    curr = buffer.PopFront();
    EXPECT_EQ('A', curr);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[B,C,D,E,F,G,H,I] (8/2)", ss.str().c_str());
    EXPECT_EQ(8, buffer.size());

    ret = buffer.PushBack('W');
    EXPECT_EQ(true, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[B,C,D,E,F,G,H,I,W] (9/1)", ss.str().c_str());

    ret = buffer.PushFront('Q');
    EXPECT_EQ(true, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[Q,B,C,D,E,F,G,H,I,W] (10/0 full)", ss.str().c_str());

    ret = buffer.PushFront('U');
    EXPECT_EQ(false, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[Q,B,C,D,E,F,G,H,I,W] (10/0 full)", ss.str().c_str());

    buffer.SetCoverable(true);

    ret = buffer.PushBack('M');
    EXPECT_EQ(false, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[B,C,D,E,F,G,H,I,W,M] (10/0 full)", ss.str().c_str());

    ret = buffer.PushFront('N');
    EXPECT_EQ(false, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[N,B,C,D,E,F,G,H,I,W] (10/0 full)", ss.str().c_str());

    ret = buffer.PushBack('R');
    EXPECT_EQ(false, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[B,C,D,E,F,G,H,I,W,R] (10/0 full)", ss.str().c_str());

    ret = buffer.PushFront('S');
    EXPECT_EQ(false, ret);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[S,B,C,D,E,F,G,H,I,W] (10/0 full)", ss.str().c_str());

    curr = buffer.PopFront();
    EXPECT_EQ('S', curr);
    curr = buffer.PopFront();
    EXPECT_EQ('B', curr);
    curr = buffer.PopFront();
    EXPECT_EQ('C', curr);
    curr = buffer.PopFront();
    EXPECT_EQ('D', curr);

    curr = buffer.PopBack();
    EXPECT_EQ('W', curr);
    curr = buffer.PopBack();
    EXPECT_EQ('I', curr);
    curr = buffer.PopBack();
    EXPECT_EQ('H', curr);
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[E,F,G] (3/7)", ss.str().c_str());
    
    buffer.PushBack('Z');
    buffer.PushBack('X');
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[E,F,G,Z,X] (5/5)", ss.str().c_str());

    buffer.Clear();
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("empty", ss.str().c_str());

    buffer.PushFront('A');
    buffer.PushBack('Z');
    buffer.PushFront('V');
    buffer.PushBack('F');
    buffer.PushFront('C');
    buffer.PushBack('D');
    buffer.PushFront('E');
    buffer.PushBack('K');
    buffer.PushFront('G');
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[G,E,C,V,A,Z,F,D,K] (9/1)", ss.str().c_str());

    EXPECT_EQ('G', buffer[0]);
    EXPECT_EQ('K', buffer[8]);
    EXPECT_EQ('E', buffer[1]);
    EXPECT_EQ('E', buffer[11]);
    EXPECT_EQ('G', buffer[10]);
    EXPECT_EQ('F', buffer[136]);
    EXPECT_EQ('A', buffer[34]);
    EXPECT_EQ('G', buffer[230]);
    EXPECT_EQ('D', buffer[237]);
    EXPECT_EQ('K', buffer[238]);
}

TEST(test_CircularBuffer, case_crush_test_1) {
    // The following operations will crash the firmware and cause a reset
    spk::CircularBuffer<char, 10> buffer;
    std::stringstream ss;
    EXPECT_EQ(buffer.type, buffer.PopFront());
    EXPECT_EQ(0, buffer.size());
    EXPECT_EQ(buffer.type, buffer.PopBack());
    EXPECT_EQ(0, buffer.size());
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("empty", ss.str().c_str());
}

TEST(test_CircularBuffer, case_reload_test_1) {
    spk::CircularBuffer<char, 10> buffer;
    buffer << 'x' << 'y' << 'z';

    std::stringstream ss;
    PRINT_BUFFER(buffer, ss);
    EXPECT_STREQ("[x,y,z] (3/7)", ss.str().c_str());

    char x = 0;
    buffer >> x;
    EXPECT_EQ('x', x);
    buffer >> x;
    EXPECT_EQ('y', x);
    buffer >> x;
    EXPECT_EQ('z', x);
    buffer >> x;
    EXPECT_EQ(0, x);
}

TEST(test_CircularBuffer, case_crush_test_2) {
    spk::CircularBuffer<char, 10> buffer;
    char x = 0;
    buffer >> x;
    std::stringstream ss;
    EXPECT_EQ(0, x);
}

#endif // SPARKLE_TEST_CIRCULAR_BUFFER_HPP_
