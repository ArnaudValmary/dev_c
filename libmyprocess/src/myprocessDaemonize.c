
#include <libmyprocess_private.h>

void myprocessDaemonize(char *pidFilename) {
    int i;
    int fd;
    char str[12];

    if (pidFilename == NULL || strcmp(pidFilename, "") == 0) {
        exit(MYPROCESS_ERR_INVALID_PIDFILE);
    }

    myprocessFork();

    setsid();

    myprocessFork();

    for (i = getdtablesize(); i >= 0; --i) {
        close(i);
    }

    i = open(MYPROCESS_DEVICE_NULL, O_RDWR);
    dup(i);
    dup(i);

    umask(022);
    chdir(MYPROCESS_DIR_TMP);

    fd = open(pidFilename, O_RDWR | O_CREAT, 0640);
    if (fd < 0) {
        exit(MYPROCESS_ERR_CREATE_PIDFILE);
    }
    if (lockf(fd, F_TLOCK, 0) < 0) {
        exit(MYPROCESS_ERR_LOCKED_PIDFILE);
    }

    snprintf(str, 12, "%d\n", getpid());
    write(fd, str, strlen(str));
}
