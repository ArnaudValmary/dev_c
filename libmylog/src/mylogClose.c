
#include <libmylog_private.h>

void mylogEnd(void) {
    if (_Global_IsMyLog && _Global_IsMyLogFile) {
        fclose(_Global_MyLogFile);
    }
}

