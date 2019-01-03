
#include <libmylog_private.h>

void _mylogAssert(char *filename, int line, char *assertion, int test) {
    mylogWarn(filename, line, assertion, test);
}

