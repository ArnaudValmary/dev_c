
#include <libmylog_private.h>

void mylogInit(const char *filename, mylog_level_t logLevel) {
    if (filename != NULL && strcmp(filename, "") != 0) {
        _Global_IsMyLog = TRUE;
        if (strcmp(filename, MYLOG_OUT_STDERR_1) == 0 || strcmp(filename, MYLOG_OUT_STDERR_2) == 0) {
            _Global_IsMyLogFile = FALSE;
            _Global_MyLogFile   = stderr;
        } else {
            _Global_MyLogFile = fopen(filename, "a");
            if (_Global_MyLogFile != NULL) {
                _Global_IsMyLogFile = TRUE;
            } else {
                _Global_IsMyLogFile = FALSE;
            }
        }
        _Global_MyLogLevel = logLevel;
    } else {
        _Global_IsMyLog = FALSE;
    }
}

