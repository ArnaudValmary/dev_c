
#include <libmythread/libmythread.h>

#include <unistd.h>
#include <sys/syscall.h>

mythread_tid_t mythreadGetTID(void) {
    return syscall(SYS_gettid);
}
