
#include <libmythread/libmythread.h>

#ifdef __GNUC__

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#endif

mythread_tid_t mythreadGetTID(void) {
#ifdef __GNUC__
    return syscall(SYS_gettid);
#else
    return thr_self();
#endif
}
