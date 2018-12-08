
#include <liblog_private.h>

void _logAssert(char *filename, int line, char *assertion, int test) {
    logWarn(filename, line, assertion, test);
}

