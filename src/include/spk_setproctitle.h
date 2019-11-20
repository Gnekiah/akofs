#ifndef SPARKLE_SET_PROCTITLE_H_
#define SPARKLE_SET_PROCTITLE_H_

#if defined(HAVE_SETPROCTITLE)
#define spk_setproctitle(argv, title) setproctitle("%s", title)

#else
#if !defined(SETPROCTITLE_USES_ENV)
#if defined(__linux__)
#define SETPROCTITLE_USES_ENV  1
int spk_setproctitle(char** argv, char* title);

#else
#define spk_setproctitle(argv, title) 0

#endif
#endif
#endif

#endif // SPARKLE_SET_PROCTITLE_H_
