#ifndef _LIBMYHOME_PRIVATE_H_
#define _LIBMYHOME_PRIVATE_H_

#include <errno.h>      // for errno
#include <stdio.h>      // for fprintf
#include <stdlib.h>     // for atol
#include <string.h>     // for memset
#include <sys/types.h>  // for connect, recv, send
#include <sys/socket.h> // for connect, recv, send
#include <netinet/in.h> // for sockaddr_in
#include <arpa/inet.h>  // for inet_addr
#include <unistd.h>     // for close, fcntl, alarm
#include <fcntl.h>      // for fcntl
#include <signal.h>     // for siginfo_t, sigaction

#include <libmyhome/libmyhome.h>

#define LIBMYHOME_BUFFER_ALLOC_SIZE (1024)

myHomePass_t myHomePass(const char *password_s, const char* nonce_s);

char*        myHomeRead(myHomeSession_t* pSession);
boolean      myHomeReadAck(myHomeSession_t* pSession);

boolean      myHomeWrite(myHomeSession_t* pSession, const char* buffer, const int bufferSize);

boolean      myHomeSignalConfig(void);
void         myHomeSignalHandler(int signum, siginfo_t* siginfo, void* voidptr);

boolean      myHomeConnect(myHomeSession_t* pSession);

#endif