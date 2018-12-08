
#include <liblog_private.h>

void logEnd(void) {
    if (_Global_IsLog && _Global_IsLogFile) {
        fclose(_Global_LogFile);
    }
}

