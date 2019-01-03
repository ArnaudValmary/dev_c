#include <libmysrv_private.h>

void mysrvFork(int sock, void *func(int sock)) {
    int pid;
    switch (pid = fork()) {
        case -1 :
            logError(__FILE__, __LINE__, 0, "Fork error. Exit");
            exit(MYSRV_ERROR_FORK);
        case 0 :
            func(sock);
            exit(MYSRV_WARN_FORK);
        default :
            close(sock);
    }
}

