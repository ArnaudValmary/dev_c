#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

char* makeSemPath(void) {
    static char path[1024];

    memset(path, 0, sizeof(path));
    strcpy(path, DIR_TMP);
    strcat(path, "/");
    strcat(path, PROJ_FILENAME);

    printf("Semaphore associated path = '%s'\n", path);

    return path;
}

key_t makeSemKey(char* path) {
    key_t semKey = ftok(path, PROJ_ID);
    printf("Server: Semaphore key = '%d'\n", semKey);
    return semKey;
}

void handcont(int signal) {
    printf("\n");
    printf("Client: Server response arrived (signal = %d)\n", signal);
}

void P(int sem ) {
    struct sembuf buf;

    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;

    semop(sem,&buf,1);
}

void V(int sem ) {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op  = 1;
    buf.sem_flg = 0;

    semop(sem,&buf,1);
}


int main(int argc, char* argv[]) {
    int segID = 0;
    int semID = 0;
    int* segAddress;
    int* adr_serveur;
    int* adr_client;
    int* adr_num;
    int* adr_nb;
    int nbDemand, object, Rep;
    int serverPID;
    key_t semKey = 0;

    memset(&semKey, 0, sizeof(semKey));
    semKey = makeSemKey(makeSemPath());

    /* Test arguments */
    if (argc != 3) {
        printf("\n");
        printf("Bad arguments\n");
        printf("Syntax : client [object (1..6)] [demands]\n");
        exit(1);
    }

    /* Get arguments */
    object   = atoi(argv[1]);
    nbDemand = atoi(argv[2]);

    /* Verify object number */
    if ( (object <1) || (object > 6)) {
        printf("\n");
        printf("Client: The object number must be between 1 and 6\n");
        exit(2);
    }

    /* Verify demand */
    if ( nbDemand < 1 ) {
        printf("\n");
        printf("Client: The demand must be greater than zero\n");
        exit(3);
    }

    segID = shmget(semKey,sizeof(SHM_TYPE)*(SHM_SIZE),0);
    if(segID == -1) {
        perror("Client: Segment not found\n");
        exit(1);
    }

    segAddress = (int*) shmat(segID, NULL, 0);
    if(segAddress == (int*) -1) {
        perror("Client: Segment attachment impossible");
        exit(2);
    }
    printf("Client: Segment attached (address = %p)\n", (void*) segAddress);

    semID = semget(semKey,1,0);
    if (semID == -1) {
        perror ("Client: Semaphore not found\n");
        exit(5);
    }
    printf("Client: Semaphore found (id = %d)\n", semID);

    adr_serveur = segAddress;
    adr_client  = segAddress+7;
    adr_num = segAddress+8;
    adr_nb = segAddress+9;

    serverPID = *adr_serveur;
    printf("Server PID = %d\n", serverPID);

    signal(SIGUSR1,handcont);

    P(semID);
    *adr_client = getpid();
    *adr_num    = object;
    *adr_nb     = nbDemand;
    kill(*adr_serveur,SIGUSR1);
    pause();
    Rep = *adr_num;
    V(semID);
    printf("\n");
    if (Rep == 1) {
        printf("Reservation OK\n");
    } else {
        printf("Reservation impossible, no free space\n");
    }

    return EXIT_SUCCESS;
}
