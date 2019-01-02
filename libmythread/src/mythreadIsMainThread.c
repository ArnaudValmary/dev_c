
#include <libmythread/libmythread.h>

#ifdef __GNUC__

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#endif

boolean_t mythreadIsMainThread(void) {
#ifdef __GNUC__
    if (syscall(SYS_gettid) == getpid()) {
        return TRUE;
    }
    return FALSE;
#else
    if (thr_self() == 1) {
        return TRUE;
    }
    return FALSE;
#endif
}
