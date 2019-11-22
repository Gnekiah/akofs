#ifndef SPARKLE_TEST_SPK_COMPAT_HPP_
#define SPARKLE_TEST_SPK_COMPAT_HPP_

#include <string>
#include <spk_compat.h>

void* thrd(void*) {
    pthread_t tid;
    tid = pthread_self();
    printf("tid:%d (0x%x)\n", tid, tid);
    while (1) {}
    return NULL;
}

TEST(test_spk_compat, case_1) {
    ASSERT_TRUE(likely(1 < 2));
    ASSERT_FALSE(likely(1 > 2));
    ASSERT_TRUE(unlikely(1 < 2));
    ASSERT_FALSE(unlikely(1 > 2));

    std::string title1 = "sparkle_thread_test_sparkle_thread_test-1";
    std::string title2 = "sparkle_thread_test_sparkle_thread_test-2";
    pthread_t pa, pb;
    char ret_title[64];
    int pl_th1 = pthread_create(&pa, NULL, thrd, NULL);
    int pl_th2 = pthread_create(&pb, NULL, thrd, NULL);
    spk_pthread_setname(pa, title1.c_str());
    spk_pthread_setname(pb, title2.c_str());
    spk_pthread_getname(pa, ret_title, 63);
    ASSERT_STREQ(ret_title, title1.c_str());
    spk_pthread_getname(pb, ret_title, 63);
    ASSERT_STREQ(ret_title, title2.c_str());

    pthread_join(pa, NULL);
    pthread_join(pb, NULL);
}

#endif // SPARKLE_TEST_SPK_COMPAT_HPP_