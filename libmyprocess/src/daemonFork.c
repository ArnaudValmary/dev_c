
#include <libdaemon_private.h>

void daemonFork(void) {
    switch (fork()) {
        case -1 :
            logError(__FILE__, __LINE__, 0, "Fork error. Exit");
            exit(ERROR_FORK);
        case 0 :
            break;
        default :
            exit(INFO_FORK_OK);
    }
}

