#ifndef _LIBMYLOG_H_
#define _LIBMYLOG_H_

#include <incboolean/libboolean.h>

#define MYLOG_LEVEL_MIN 0
#define MYLOG_LEVEL_MAX 4

typedef enum {
    MYLOG_LEVEL_FATAL =  0,
    MYLOG_LEVEL_ERROR =  1,
    MYLOG_LEVEL_WARN  =  2,
    MYLOG_LEVEL_INFO  =  3,
    MYLOG_LEVEL_DEBUG =  4
} mylog_level_t;

#define MYLOG_TEXT_DEFAULT "UNKNOWN"

extern char *MYLOG_TEXT[];

#define MYLOG_OUT_STDERR_1         "STDERR"
#define MYLOG_OUT_STDERR_2         "-"
#define MYLOG_DATE_FORMAT          "%04d.%02d.%02dT%02d:%02d:%02d"
#define MYLOG_FORMATED_DATE_LENGTH 19

void mylogInit(const char *filename, mylog_level_t level);
void mylogEnd(void);

void mylogFatal(char *filename, int lineNumber, char *text, ...);
void mylogError(char *filename, int lineNumber, char *text, ...);
void mylogWarn (char *filename, int lineNumber, char *text, ...);
void mylogInfo (char *filename, int lineNumber, char *text, ...);
void mylogDebug(char *filename, int lineNumber, char *text, ...);

void _mylogAssert(char *filename, int line, char *assertion, int test);

#define mylogAssert(a) _mylogAssert(__FILE__, __LINE__, #a, a)

#endif
