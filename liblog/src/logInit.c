
#include <liblog_private.h>

void logInit(const char *filename, tLogLevel logLevel) {
    if (filename != NULL && strcmp(filename, "") != 0) {
        _Global_IsLog = TRUE;
        if (strcmp(filename, LOG_OUT_STDERR_1) == 0 || strcmp(filename, LOG_OUT_STDERR_2) == 0) {
            _Global_IsLogFile = FALSE;
            _Global_LogFile   = stderr;
        } else {
            _Global_LogFile = fopen(filename, "a");
            if (_Global_LogFile != NULL) {
                _Global_IsLogFile = TRUE;
            } else {
                _Global_IsLogFile = FALSE;
            }
        }
        _Global_LogLevel = logLevel;
    } else {
        _Global_IsLog = FALSE;
    }
}

