#ifndef AKOFS_TEST_AKO_COMPAT_HPP_
#define AKOFS_TEST_AKO_COMPAT_HPP_

#include <string>
#include <ako_compat.h>

#if defined(__linux__)
#include <unistd.h>
#define msleep(t) usleep(t * 1000)
#else
#include <Windows.h>
#define msleep(t) Sleep(t)
#endif 

void* thrd(void*) {
    pthread_t tid;
    tid = pthread_self();
    // printf("tid:%d (0x%x)\n", tid, tid);
    msleep(100);
    return NULL;
}

TEST(test_ako_compat, case_1) {
    ASSERT_TRUE(likely(1 < 2));
    ASSERT_FALSE(likely(1 > 2));
    ASSERT_TRUE(unlikely(1 < 2));
    ASSERT_FALSE(unlikely(1 > 2));

    std::string title1 = "akothrd-1";
    std::string title2 = "akothrd-2";
    pthread_t pa, pb;
    char ret_title[64];
    int pl_th1 = pthread_create(&pa, NULL, thrd, NULL);
    int pl_th2 = pthread_create(&pb, NULL, thrd, NULL);
    ako_pthread_setname(pa, title1.c_str());
    ako_pthread_setname(pb, title2.c_str());
    ako_pthread_getname(pa, ret_title, 63);
#ifdef __linux__
    ASSERT_STREQ(ret_title, title1.c_str());
#else
    ASSERT_STREQ(title1.c_str(), title1.c_str());
#endif
    ako_pthread_getname(pb, ret_title, 63);
#ifdef __linux__
    ASSERT_STREQ(ret_title, title2.c_str());
#else
    ASSERT_STREQ(title2.c_str(), title2.c_str());
#endif
    pthread_join(pa, NULL);
    pthread_join(pb, NULL);
}

#endif // AKOFS_TEST_AKO_COMPAT_HPP_
