#ifndef _LIBMYPROCESS_H_
#define _LIBMYPROCESS_H_

#define MYPROCESS_ERR_FORK            40
#define MYPROCESS_OK_FORK              0
#define MYPROCESS_ERR_INVALID_PIDFILE 50
#define MYPROCESS_ERR_CREATE_PIDFILE  52
#define MYPROCESS_ERR_LOCKED_PIDFILE  53

void myprocessDaemonize(char *pidFilename);
void myprocessFork(void);

#endif

