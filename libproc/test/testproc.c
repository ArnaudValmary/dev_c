
/**
 * For printf function
 */
#include <stdio.h>

/**
 * For procGetProcessName function
 */
#include <libproc/libproc.h>

int main(void) {
    printf("%s v0.1\n", procGetProcessName());
    return 0;
}
