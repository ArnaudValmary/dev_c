#include <libmysrv_private.h>

int mysrvTCPListen(int port, int maxNbClient) {
    int sock;
    int optval = -1;
    struct sockaddr_in sockname;

    if ((sock = socket(PF_INET /* IPv4*/, SOCK_STREAM /* TCP */, 0)) < 0) {
        logError(__FILE__, __LINE__, 0, "Socket opening error. Exit");
        exit(100);
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
    memset((char*) &sockname, 0, sizeof(struct sockaddr_in));
    sockname.sin_family      = AF_INET;
    sockname.sin_port        = htons(port);
    sockname.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *) &sockname, sizeof(struct sockaddr_in)) < 0) {
        logError(__FILE__, __LINE__, 0, "Bind error. Exit");
        exit(MYSRV_ERROR_BIND);
    }

    if (listen(sock, maxNbClient) < 0) {
        logError(__FILE__, __LINE__, 0, "Listen error. Exit");
        exit(MYSRV_ERROR_LISTEN);
    }

    return sock;
}

