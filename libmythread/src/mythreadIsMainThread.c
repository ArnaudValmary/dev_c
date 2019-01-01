
#include <libmythread/libmythread.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

boolean_t mythreadIsMainThread(void) {
    if (syscall(SYS_gettid) == getpid()) {
        return TRUE;
    }
    return FALSE;
}
