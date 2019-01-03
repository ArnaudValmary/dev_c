
#include <libmyprocess_private.h>

void myprocessFork(void) {
    switch (fork()) {
        case -1 :
            mylogError(__FILE__, __LINE__, 0, "Fork error. Exit");
            exit(MYPROCESS_ERR_FORK);
        case 0 :
            break;
        default :
            exit(MYPROCESS_OK_FORK);
    }
}

