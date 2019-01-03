#include <libmysrv_private.h>

void mysrvCleanClient(void) {
    int pid;
    while ((pid = waitpid(-1, NULL, WNOHANG))) {
        if (pid == -1) {
            mylogError(__FILE__, __LINE__, 0, "WaitPID error. Exit");
            exit(MYSRV_ERROR_WAITPID);
        }
    }
}

