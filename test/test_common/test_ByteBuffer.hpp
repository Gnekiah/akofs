#ifndef SPARKLE_TEST_BYTE_BUFFER_HPP_
#define SPARKLE_TEST_BYTE_BUFFER_HPP_

#include <gtest/gtest.h>
#include <ByteBuffer.hpp>

TEST(test_ByteBuffer, case_create_implementation_test_1) {
    const char* strbuf = "And live the day as if your days are numbered.";
    const int strbuf_size = strlen(strbuf);

    auto bb1 = std::make_unique<spk::ByteBuffer>();
    auto bb2 = std::make_unique<spk::ByteBuffer>(30);
    auto bb3 = std::make_unique<spk::ByteBuffer>((uint8_t*)strbuf, strbuf_size);
    auto bb4 = std::make_unique<spk::ByteBuffer>(*bb3.get());
    auto bb5 = bb4->Clone();
    spk::ByteBuffer bb6 = *bb5.get();
    spk::ByteBuffer bb7 = bb6;

    EXPECT_EQ(30, bb2->capacity());
    EXPECT_EQ(0, bb2->BytesRemaining());
    EXPECT_EQ(strbuf_size, bb3->size());
    EXPECT_EQ(strbuf_size, bb3->capacity());
    EXPECT_EQ(strbuf_size, bb3->BytesRemaining());
    EXPECT_EQ('A', bb3->Peek());
    EXPECT_EQ('A', bb3->Peek());
    EXPECT_EQ('d', bb6[2]);
    EXPECT_EQ('l', bb6[4]);
    EXPECT_EQ('e', bb7[7]);
    EXPECT_EQ('h', bb7[10]);

    bb3->Resize(28);
    EXPECT_EQ(0, bb3->write_pos());
    EXPECT_EQ(0, bb3->read_pos());
    EXPECT_EQ(28, bb3->size());
    bb3->Resize(64);
    EXPECT_EQ(0, bb3->write_pos());
    EXPECT_EQ(0, bb3->read_pos());
    EXPECT_EQ(64, bb3->size());

    EXPECT_FALSE(bb3->Equals(*bb4.get()));
    EXPECT_TRUE(bb4->Equals(*bb5.get()));
    EXPECT_TRUE(bb4->Equals(*bb5.get()));

    EXPECT_TRUE(bb6 == bb7);
    
    bb4->Clear();
    bb4->Resize(64);
    EXPECT_EQ(64, bb4->size());

    bb7.set_read_pos(10086);
    bb7.set_write_pos(10086);
    EXPECT_EQ(10086 % bb7.size(), bb7.read_pos());
    EXPECT_EQ(10086 % bb7.size(), bb7.write_pos());
}

TEST(test_ByteBuffer, case_create_implementation_and_get_bytes_test_1) {
    const char* strbuf = "And live the day as if your days are numbered.";
    const int strbuf_size = strlen(strbuf);
    char buff[128];

    spk::ByteBuffer bb1((uint8_t*)strbuf, strbuf_size);
    EXPECT_EQ(4, bb1.Find('l', 0));
    EXPECT_EQ(1, bb1.Find('n', 0));
    EXPECT_EQ(37, bb1.Find('n', 4));
    EXPECT_EQ(-1, bb1.Find('X'));

    bb1.Replace('i', 'I', 0, true);
    bb1.Replace('d', 'D', 4, true);
    bb1.Replace('e', 'E', 8);

    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 127);
    EXPECT_STREQ("And lIve thE Day as if your days arE numbErEd.", buff);

    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 127);
    EXPECT_STREQ("", buff);

    bb1.set_read_pos(0);
    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 127, false);
    EXPECT_STREQ("And lIve thE Day as if your days arE numbErEd.", buff);

    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 127, false);
    EXPECT_STREQ("And lIve thE Day as if your days arE numbErEd.", buff);

    memset(buff, 0, 128);
    bb1.GetBytesFrom((uint8_t*)buff, 127, 12);
    EXPECT_STREQ(" Day as if your days arE numbErEd.", buff);

    EXPECT_EQ('A', bb1.Peek());
    EXPECT_EQ('A', bb1.Get<char>());
    EXPECT_EQ('n', bb1.Get<char>());
    EXPECT_EQ('d', bb1.Get<char>());
    EXPECT_EQ(' ', bb1.Get<char>());

    uint32_t _shift = (uint8_t)'e';
    _shift = (_shift << 8) | (uint8_t)'v';
    _shift = (_shift << 8) | (uint8_t)'I';
    _shift = (_shift << 8) | (uint8_t)'l';
    EXPECT_EQ(_shift, bb1.Get<uint32_t>());

    EXPECT_EQ('E', bb1.Get<char>(11));
    EXPECT_EQ(0, bb1.Get<char>(11111));
}

TEST(test_ByteBuffer, case_byte_buffer_put_bytes_test_1) {
    char buff[128];
    spk::ByteBuffer bb1(64);

    bb1.Put('S');
    bb1.Put('P');
    bb1.Put('a');
    bb1.Put('r');
    bb1.Put('k');
    bb1.Put('l');
    bb1.Put('e');

    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 128, false);
    EXPECT_STREQ("SParkle", buff);

    bb1.Put('p', 1);
    bb1.PutBytes((uint8_t*)"arkle test", 10);
    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 128, false);
    EXPECT_STREQ("Sparkle test", buff);

    bb1.PutBytes((uint8_t*)"Framework", 9, 8);
    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 128, false);
    EXPECT_STREQ("Sparkle Framework", buff);

    spk::ByteBuffer bb2(64);
    bb2.PutByteBuffer(bb1);
    EXPECT_TRUE(bb1 == bb2);
}

TEST(test_ByteBuffer, case_byte_buffer_stream_functional_test_1) {
    char buff[128];
    spk::ByteBuffer bb1(64);

    bb1 << "Sparkle Test";
    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 128, false);
    EXPECT_STREQ("Sparkle Test", buff);

    bb1 << std::string(" Sparkle Framework");
    memset(buff, 0, 128);
    bb1.GetBytes((uint8_t*)buff, 128, false);
    EXPECT_STREQ("Sparkle Test Sparkle Framework", buff);

    bb1.Clear();
    bb1 << (uint16_t)10086;
    uint16_t x = 0;
    bb1 >> x;
    EXPECT_EQ(10086, x);
}

TEST(test_ByteBuffer, case_byte_buffer_stream_functional_test_2) {
    char buff[128];
    spk::ByteBuffer bb1(64);
    struct s_t {
        char i;
        long j;
        double k;
        uint64_t* v;
        void* u;
    } st = {
        12,
        100235,
        234.45,
        (uint64_t*)& buff[0],
        &bb1
    };

    struct s_t st2;

    bb1 << st;
    bb1 >> st2;
    EXPECT_EQ(st.i, st2.i);
    EXPECT_EQ(st.j, st2.j);
    EXPECT_NEAR(st.k, st2.k, 0.1);
    EXPECT_EQ((uint64_t)st.v, (uint64_t)st2.v);
    EXPECT_EQ((uint64_t)st.u, (uint64_t)st2.u);
}

TEST(test_ByteBuffer, case_byte_buffer_packed_and_unpacked_functional_test_1) {
    char buff[128];
    spk::ByteBuffer bb1(64);

    uint64_t q = 0, w = 0, e = 0, r = 0, t = 0;
    bb1.Packed((uint64_t)23425, (uint64_t)34531, (uint64_t)8745, (uint64_t)80678, (uint64_t)12034);
    bb1.Unpacked(q, w, e, r, t);

    EXPECT_EQ(23425, q);
    EXPECT_EQ(34531, w);
    EXPECT_EQ(8745, e);
    EXPECT_EQ(80678, r);
    EXPECT_EQ(12034, t);
}

#endif // SPARKLE_TEST_BYTE_BUFFER_HPP_