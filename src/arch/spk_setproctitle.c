#include <spk_setproctitle.h>

#if defined(SETPROCTITLE_USES_ENV)

#include <string.h>
#include <stdlib.h>

extern char** environ;

int spk_setproctitle(char** argv, char* title) {
    char* last = nullptr, *p = nullptr;
    size_t size = 0;
    uint32_t i;

    for (i = 0; environ[i]; i++) {
        size += strlen(environ[i]) + 1;
    }

    p = (char*)malloc(size);
    if (!p) {
        return -1;
    }

    last = argv[0];
    for (i = 0; argv[i]; i++) {
        last += strlen(argv[i]) + 1;
    }

    for (i = 0; environ[i]; i++) {
        size = strlen(environ[i]) + 1;
        last += size;

        strncpy(p, environ[i], size);
        environ[i] = p;
        p += size;
    }

    last--;
    argv[1] = NULL;
    p = strncpy(argv[0], title, last - argv[0]);
    p += strlen(title);
    if (last - p) {
        memset(p, '\0', last - p);
    }
    return 0;
}

#endif