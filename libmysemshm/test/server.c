#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define IDX_SERVER_PID 0
#define NB_OBJECT      6
#define IDX_CLIENT_PID ((NB_OBJECT)      + 1)
#define IDX_OBJECT     ((IDX_CLIENT_PID) + 1)
#define IDX_DEMAND     ((IDX_OBJECT)     + 1)
#define SHM_SIZE       (IDX_DEMAND       + 1)
#define SHM_TYPE       int
#define MAX_PER_OBJECT 100

int semID = 0;
int segID = 0;

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

void handlerClient(int signal) {
    printf("\n");
    printf("Server: Received signal '%d == %s' for client\n", signal, strsignal(signal));
}


void handlerTerminate(int signal) {
    shmctl(segID, IPC_RMID, NULL);
    printf("\n");
    printf("Server: Received signal '%d == %s' for terminate server\n", signal, strsignal(signal));
    if (semctl(semID, 0, IPC_RMID, 0)==-1) {
        perror("Server: Cannot destruct semaphore");
    } else {
        printf("Server: Semaphore (id = %d) destructed\n", semID);
    }
    exit(EXIT_SUCCESS);
}

int main(void) {
    int clientPID = 0;
    int idxObject = 0;
    int nbDemand = 0;
    int* segAddress = NULL;
    key_t semKey = 2001;
    //memset(&semKey, 0, sizeof(semKey));

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
        printf("Server: Pause (wait for signal)...\n");
        fflush(stdout);
        pause();
        printf("Server: ...return from pause\n");
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

    return EXIT_SUCCESS;
}
