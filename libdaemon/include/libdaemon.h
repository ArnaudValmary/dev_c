#ifndef _LIBDAEMON_H_
#define _LIBDAEMON_H_

#define ERROR_FORK                      40
#define INFO_FORK_OK                     0
#define ERROR_DAEMONIZE_INVALID_PIDFILE 50
#define ERROR_DAEMONIZE_CREATE_PIDFILE  52
#define ERROR_DAEMONIZE_LOCKED_PIDFILE  53

void daemonDaemonize(char *pidFilename);
void daemonFork(void);

#endif

