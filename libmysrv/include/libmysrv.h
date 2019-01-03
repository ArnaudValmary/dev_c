#ifndef _LIBMYSRV_H_
#define _LIBMYSRV_H_

#define MYSRV_ERROR_BIND    10
#define MYSRV_ERROR_LISTEN  11
#define MYSRV_ERROR_ACCEPT  20
#define MYSRV_ERROR_WAITPID 30
#define MYSRV_ERROR_FORK    40
#define MYSRV_WARN_FORK     41

int mysrvTCPListen(int port, int maxNbClient);

int mysrvTCPAccept(int mainSock, int timeout);

void mysrvFork(int sock, void *func(int sock));

void mysrvCleanClient(void);

#endif
