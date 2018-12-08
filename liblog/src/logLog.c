
#include <liblog_private.h>

char *LOG_TEXT[] = {
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"
};

FILE *_Global_LogFile      = NULL;

boolean_t _Global_IsLog     = TRUE;
boolean_t _Global_IsLogFile = FALSE;

tLogLevel _Global_LogLevel = LOG_LEVEL_DEBUG;

struct tm *_Global_Timestamp;
time_t _Global_currentTime;
char _Global_FormatedDate[LOG_FORMATED_DATE_LENGTH + 1];

void _log(char *filename, int lineNumber, tLogLevel level, char *text, ...) {
    if (_Global_IsLog && level <= _Global_LogLevel) {
        va_list ap;
        int d;
        char c;
        char *s;
        _Global_currentTime = time(0);
        _Global_Timestamp = localtime(&_Global_currentTime);
        snprintf(_Global_FormatedDate, sizeof(_Global_FormatedDate), LOG_DATE_FORMAT,
                 _Global_Timestamp->tm_year + 1900, _Global_Timestamp->tm_mon + 1, _Global_Timestamp->tm_mday,
                 _Global_Timestamp->tm_hour       , _Global_Timestamp->tm_min    , _Global_Timestamp->tm_sec);
        fprintf(_Global_LogFile, "%s:%s:%s:%d",
                _Global_FormatedDate,
                level < LOG_LEVEL_MIN || level > LOG_LEVEL_MAX ? LOG_TEXT_DEFAULT : LOG_TEXT[level],
                filename, lineNumber);
        va_start(ap, text);
        while (text[0]) {
            switch (text[0]) {
                case '%' :
                    text++;
                    if (text[0]) {
                        switch (text[0]) {
                            case '%' :
                                fprintf(_Global_LogFile, "%%");
                                break;
                            case 's' :
                                s = va_arg(ap, char *);
                                fprintf(_Global_LogFile, "%s", s);
                                break;
                            case 'd' :
                                d = va_arg(ap, int);
                                fprintf(_Global_LogFile, "%d", d);
                                break;
                            case 'c' :
                                c = va_arg(ap, int);
                                fprintf(_Global_LogFile, "%c", c);
                                break;
                            default :
                                fprintf(_Global_LogFile, "%c", text[0]);
                                break;
                        }
                    } else {
                        fprintf(_Global_LogFile, "%%");
                    }
                    break;
                case '\\' :
                    text++;
                    if (text[0]) {
                        switch (text[0]) {
                            case '\\' :
                                fprintf(_Global_LogFile, "\\");
                                break;
                            case 't' :
                                fprintf(_Global_LogFile, "\t");
                                break;
                            case 'n' :
                                fprintf(_Global_LogFile, "///");
                                break;
                            case 'r' :
                                fprintf(_Global_LogFile, "///");
                                break;
                            default :
                                fprintf(_Global_LogFile, "\\%c", text[0]);
                                break;
                        }
                    } else {
                        fprintf(_Global_LogFile, "\\");
                    }
                    break;
            }
            text++;
        }
        va_end(ap);
        fprintf(_Global_LogFile, "\n");
    }
}

void logFatal(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, LOG_LEVEL_FATAL, text);
}

void logError(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, LOG_LEVEL_ERROR, text);
}

void logWarn(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, LOG_LEVEL_WARN, text);
}

void logInfo(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, LOG_LEVEL_INFO, text);
}

void logDebug(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, LOG_LEVEL_DEBUG, text);
}
