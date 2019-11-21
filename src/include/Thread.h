#ifndef SPARKLE_THREAD_H_
#define SPARKLE_THREAD_H_

#include <pthread.h>

namespace spk {
    class Thread {
    public:
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;
        Thread();
        virtual ~Thread() {}

        const pthread_t& thread_id() const;
        pid_t pid() const { return pid_; }
        bool IsStarted() const;
        bool IsSelf() const;
        int Kill(int signal);
        int TryCreate(size_t stacksize);
        void Create(const char* name, size_t stacksize = 0);
        int Join(void** prval = 0);
        int Detach();
        int SetAffinity(int cpuid);

    protected:
        virtual void* Entry() = 0;

    private:
        pthread_t thread_id_;
        pid_t pid_;
        int cpuid_;
        const char* thread_name_;
    };
}

#endif // SPARKLE_THREAD_H_