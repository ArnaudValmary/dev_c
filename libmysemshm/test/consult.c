#include <testsemshm_private.h>

char* makeSemPath(void) {
    static char path[1024];

    memset(path, 0, sizeof(path));
    strcpy(path, DIR_TMP);
    strcat(path, "/");
    strcat(path, PROJ_FILENAME);

    printf("Consult: Semaphore associated path = '%s'\n", path);

    return path;
}

key_t makeSemKey(char* path) {
    key_t semKey = ftok(path, PROJ_ID);
    printf("Consult: Semaphore key = '%d'\n", semKey);
    return semKey;
}

int main(void) {
    int* segAddress = NULL;
    int segID = 0;
    int idxObject = 0;

    key_t semKey = 0;
    memset(&semKey, 0, sizeof(semKey));
    semKey = makeSemKey(makeSemPath());

    while(((segID = shmget(semKey,sizeof(int)*10,0))!=-1) && ((segAddress = (int *) shmat(segID,NULL,0))!=(int*) -1)) {
        printf("\n");
        for (idxObject = 1; idxObject <= 6; idxObject++) {
            printf("Consult: Object #%d, %d free spaces\n", idxObject, segAddress[idxObject]);
        }
        sleep(5);
    }
    perror("Consult: Server stopped!");
    return EXIT_SUCCESS;
}
