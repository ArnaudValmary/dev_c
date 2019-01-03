
#include <libmylog_private.h>

char *MYLOG_TEXT[] = {
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"
};

FILE *_Global_MyLogFile      = NULL;

boolean_t _Global_IsMyLog     = TRUE;
boolean_t _Global_IsMyLogFile = FALSE;

mylog_level_t _Global_MyLogLevel = MYLOG_LEVEL_DEBUG;

struct tm *_Global_Timestamp;
time_t _Global_currentTime;
char _Global_FormatedDate[MYLOG_FORMATED_DATE_LENGTH + 1];

void _log(char *filename, int lineNumber, mylog_level_t level, char *text, ...) {
    if (_Global_IsMyLog && level <= _Global_MyLogLevel) {
        va_list ap;
        int d;
        char c;
        char *s;
        _Global_currentTime = time(0);
        _Global_Timestamp = localtime(&_Global_currentTime);
        snprintf(_Global_FormatedDate, sizeof(_Global_FormatedDate), MYLOG_DATE_FORMAT,
                 _Global_Timestamp->tm_year + 1900, _Global_Timestamp->tm_mon + 1, _Global_Timestamp->tm_mday,
                 _Global_Timestamp->tm_hour       , _Global_Timestamp->tm_min    , _Global_Timestamp->tm_sec);
        fprintf(_Global_MyLogFile, "%s:%s:%s:%d",
                _Global_FormatedDate,
                level < MYLOG_LEVEL_MIN || level > MYLOG_LEVEL_MAX ? MYLOG_TEXT_DEFAULT : MYLOG_TEXT[level],
                filename, lineNumber);
        va_start(ap, text);
        while (text[0]) {
            switch (text[0]) {
                case '%' :
                    text++;
                    if (text[0]) {
                        switch (text[0]) {
                            case '%' :
                                fprintf(_Global_MyLogFile, "%%");
                                break;
                            case 's' :
                                s = va_arg(ap, char *);
                                fprintf(_Global_MyLogFile, "%s", s);
                                break;
                            case 'd' :
                                d = va_arg(ap, int);
                                fprintf(_Global_MyLogFile, "%d", d);
                                break;
                            case 'c' :
                                c = va_arg(ap, int);
                                fprintf(_Global_MyLogFile, "%c", c);
                                break;
                            default :
                                fprintf(_Global_MyLogFile, "%c", text[0]);
                                break;
                        }
                    } else {
                        fprintf(_Global_MyLogFile, "%%");
                    }
                    break;
                case '\\' :
                    text++;
                    if (text[0]) {
                        switch (text[0]) {
                            case '\\' :
                                fprintf(_Global_MyLogFile, "\\");
                                break;
                            case 't' :
                                fprintf(_Global_MyLogFile, "\t");
                                break;
                            case 'n' :
                                fprintf(_Global_MyLogFile, "///");
                                break;
                            case 'r' :
                                fprintf(_Global_MyLogFile, "///");
                                break;
                            default :
                                fprintf(_Global_MyLogFile, "\\%c", text[0]);
                                break;
                        }
                    } else {
                        fprintf(_Global_MyLogFile, "\\");
                    }
                    break;
            }
            text++;
        }
        va_end(ap);
        fprintf(_Global_MyLogFile, "\n");
    }
}

void mylogFatal(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, MYLOG_LEVEL_FATAL, text);
}

void mylogError(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, MYLOG_LEVEL_ERROR, text);
}

void mylogWarn(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, MYLOG_LEVEL_WARN, text);
}

void mylogInfo(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, MYLOG_LEVEL_INFO, text);
}

void mylogDebug(char *filename, int lineNumber, char *text, ...) {
    _log(filename, lineNumber, MYLOG_LEVEL_DEBUG, text);
}
