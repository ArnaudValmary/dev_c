#ifndef _TESTMYSEMSHM_PRIVATE_H_
#define _TESTMYSEMSHM_PRIVATE_H_

#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define IDX_SERVER_PID 0
#define NB_OBJECT      6
#define IDX_CLIENT_PID ((NB_OBJECT)      + 1)
#define IDX_OBJECT     ((IDX_CLIENT_PID) + 1)
#define IDX_DEMAND     ((IDX_OBJECT)     + 1)
#define SHM_SIZE       (IDX_DEMAND       + 1)
#define SHM_TYPE       int
#define MAX_PER_OBJECT 100
#define PROJ_ID        1234509876
#define DIR_TMP        "/var/tmp"
#define PROJ_FILENAME  "mysemshm_test"

#endif