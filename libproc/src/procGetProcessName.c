/**
 * For pid_t type
 * For getpid function
 */
#include <sys/types.h>
#include <unistd.h>

/**
 * For memset function
 */
#include <string.h>

/**
 * For snprintf, fopen, fread and fclose functions
 */
#include <stdio.h>

/**
 * This local lib
 */
#include <libproc/libproc.h>

/**
 * RETURN : the name of the process program filename (static)
 */
char* procGetProcessName(void) {

    static int flagName = 0;
    static char name[1024];

    if (flagName == 0) {
        pid_t pid = getpid();
        snprintf(name, sizeof(name) - sizeof(char), "/proc/%d/comm", pid);
        FILE* f = fopen(name,"r");
        if (f) {
            size_t size = fread(name, sizeof(char), sizeof(name) - sizeof(char), f);
            if (size > 0) {
                if ('\n' == name[size - 1]) {
                    name[size-1]='\0';
                }
                flagName = 1;
            } else {
                memset(name, 0, sizeof(name));
            }
            fclose(f);
        } else {
            memset(name, 0, sizeof(name));
        }
    }
    return name;
}