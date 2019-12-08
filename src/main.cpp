#include <iostream>
#include <string>
#include <spk_compat.h>
#include <spk_setproctitle.h>

static void usage() 
{
    std::cerr << "usage: sparkle -i "
        << ""
        << std::endl;
}

int main(int argc, const char **argv) 
{
    spk_setproctitle(argv, "sparkle");
    spk_pthread_setname(pthread_self(), "sparkle");
    int err = 0;



    return 0;
}