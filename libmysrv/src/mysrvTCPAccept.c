#include <libmysrv_private.h>

int mysrvTCPAccept(int mainSock, int timeout) {
    int sock;

    if (timeout > 0) {
        alarm(timeout);
    }

    if ((sock = accept(mainSock, NULL, 0)) < 0) {
        if (errno = EINTR) {
            shutdown(mainSock, SHUT_RDWR);
            close(mainSock);
            if (timeout > 0) {
                alarm(0);
                return -1;
            }
        } else {
            logError(__FILE__, __LINE__, 0, "Accept error. Exit");
            exit(MYSRV_ERROR_ACCEPT);
        }
    }

    if (timeout > 0) {
        alarm(0);
    }

    fcntl(sock, F_SETFD, 1);

    return sock;
}

