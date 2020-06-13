#ifndef MANDIS_MAGIC_H_
#define MANDIS_MAGIC_H_

#include <atomic>

/* While fatal error occurred, set the flag and the main thread'll exiting */
static std::atomic_bool ExitFlag = { false };

#endif // !MANDIS_MAGIC_H_

