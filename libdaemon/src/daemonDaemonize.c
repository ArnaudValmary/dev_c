
#include <libdaemon_private.h>

void daemonDaemonize(char *pidFilename) {
    int i;
    int fd;
    char str[12];

    if (pidFilename == NULL || strcmp(pidFilename, "") == 0) {
        exit(ERROR_DAEMONIZE_INVALID_PIDFILE);
    }

    daemonFork();

    setsid();

    daemonFork();

    for (i = getdtablesize(); i >= 0; --i) {
        close(i);
    }

    i = open(DEVICE_NULL, O_RDWR);
    dup(i);
    dup(i);

    umask(022);
    chdir(DAEMON_BASE_DIR);

    fd = open(pidFilename, O_RDWR | O_CREAT, 0640);
    if (fd < 0) {
        exit(ERROR_DAEMONIZE_CREATE_PIDFILE);
    }
    if (lockf(fd, F_TLOCK, 0) < 0) {
        exit(ERROR_DAEMONIZE_LOCKED_PIDFILE);
    }

    snprintf(str, 12, "%d\n", getpid());
    write(fd, str, strlen(str));
}
