#include <testsemshm_private.h>

typedef enum {STOP, RUN, TERM_SIGNAL, CLIENT_SIGNAL} serverState_t;

int semID = 0;
int segID = 0;
serverState_t serverState = STOP;

char* makeSemPath(void) {
    static char semPath[1024];

    memset(semPath, 0, sizeof(semPath));
    strcpy(semPath, DIR_TMP);
    strcat(semPath, "/");
    strcat(semPath, PROJ_FILENAME);

    printf("Server: Semaphore associated path = '%s'\n", semPath);

    FILE* pidFile = fopen(semPath, "w+x");
    if (pidFile == NULL) {
        if (errno == EEXIST) {
            perror("Server: PID file already exists");
        } else {
            perror("Server: Cannot open PID file");
        }
        return NULL;
    } else {
        char strPID[15];
        snprintf(strPID, sizeof(strPID) - 1, "%d", getpid());
        fwrite(strPID, strlen(strPID), 1, pidFile);
        fclose(pidFile);
        printf("Server: PID (%s) written in PID file (%s)\n", strPID, semPath);
    }

    return semPath;
}

key_t makeSemKey(char* path) {
    key_t semKey = ftok(path, PROJ_ID);
    printf("Server: Semaphore key = '%d'\n", semKey);
    return semKey;
}

void initializeSemaphore(key_t semKey) {

    semID = semget(semKey, 1, IPC_CREAT|IPC_EXCL|0600);

    if (semID == -1) {
        perror("Server: Cannot create semaphore");
        semID = semget(semKey, 1, 0600);
        if (semID == -1) {
            perror("Server: Cannot read semaphore");
        } else {
            printf("Server: Semaphore ID = %d\n", semID);
        }
    } else {
        printf("Server: Semaphore created (id = %d)\n", semID);
        if ( semctl(semID, 0, SETVAL, 1) == -1 ) {
            perror("Server: Cannot set semaphore value");
        } else {
            printf("Server: Initialization OK\n");
        }
    }
}

void destructSemaphore(int semID, char* semPath) {
    if (semctl(semID, 0, IPC_RMID, 0) == -1) {
        perror("Server: Cannot destruct semaphore");
    } else {
        printf("Server: Semaphore (id = %d) destructed\n", semID);
    }
    if (unlink(semPath) == -1) {
        perror("Server: Cannot destruct PID file");
    } else {
        printf("Server: PID file (%s) destructed\n", semPath);
    }
}

void handlerClient(int signal) {
    printf("\n");
    printf("Server: Received signal '%d == %s' for client\n", signal, strsignal(signal));
    serverState = CLIENT_SIGNAL;
}

void handlerTerminate(int signal) {
    printf("\n");
    printf("Server: Received signal '%d == %s' for terminate server\n", signal, strsignal(signal));
    serverState = TERM_SIGNAL;
}

int main(void) {
    int clientPID = 0;
    int idxObject = 0;
    int nbDemand = 0;
    int* segAddress = NULL;
    key_t semKey = 0;
    char* semPath = makeSemPath();

    memset(&semKey, 0, sizeof(semKey));
    semKey = makeSemKey(semPath);

    segID = shmget(semKey, sizeof(SHM_TYPE) * (SHM_SIZE), IPC_CREAT | IPC_EXCL | 0600);
    if(segID == -1) {
        perror("Server: Segment creation impossible");
        return 1;
    }
    printf("Server: Segment created (id = %d)\n", segID);

    segAddress = (int*) shmat(segID, NULL, 0);
    if(segAddress == (int*) -1) {
        perror("Server: Segment attachment impossible");
        return 2;
    }
    printf("Server: Segment attached (address = %p)\n", (void*) segAddress);

    for (idxObject = 1; idxObject <= NB_OBJECT; idxObject++) { /* Initialisation à max-places des */
        segAddress[idxObject] = MAX_PER_OBJECT;   /* nombres de places disponibles   */
    }

    segAddress[IDX_SERVER_PID] = getpid();
    printf("Server: Server PID = %d\n", segAddress[IDX_SERVER_PID]);

    signal(SIGUSR1, handlerClient);
    signal(SIGINT,  handlerTerminate);

    initializeSemaphore(semKey);

    printf("Server: Server ready!\n");

    while(1) {
        if (serverState == TERM_SIGNAL) {
            break;
        }
        serverState = RUN;
        printf("Server: Pause (wait for signal)...\n");
        fflush(stdout);
        pause();
        printf("Server: ...return from pause\n");
        switch (serverState) {
            case CLIENT_SIGNAL: {
                serverState = RUN;
                break;
            }
            case TERM_SIGNAL: {
                destructSemaphore(semID, semPath);
                return EXIT_SUCCESS;
            }
            default: {
                // WTF ?
                continue;
            }
        }
        clientPID = segAddress[IDX_CLIENT_PID];
        printf("Server : Client PID '%d'\n", clientPID);
        idxObject = segAddress[IDX_OBJECT];
        printf("Server : Object is '%d'\n", idxObject);
        nbDemand = segAddress[IDX_DEMAND];
        printf("Server : Demand is '%d'\n", nbDemand);

        if ( nbDemand <= segAddress[idxObject] ) {
            segAddress[idxObject]  -= nbDemand;
            segAddress[IDX_OBJECT]  = 1;
        } else {
            segAddress[IDX_OBJECT]  = 0;
        }
        printf("Server : For object '%d' free place is '%d'\n", idxObject, segAddress[idxObject]);

        printf("Server : Send signal '%d' to client PID '%d'\n", SIGUSR1, clientPID);
        kill(clientPID, SIGUSR1);
    }

    destructSemaphore(semID, semPath);

    return EXIT_SUCCESS;
}
