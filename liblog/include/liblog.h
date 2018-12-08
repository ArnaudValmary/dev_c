#ifndef _LIBLOG_H_
#define _LIBLOG_H_

#include <incboolean/libboolean.h>

#define LOG_LEVEL_MIN 0
#define LOG_LEVEL_MAX 4

typedef enum {
    LOG_LEVEL_FATAL =  0,
    LOG_LEVEL_ERROR =  1,
    LOG_LEVEL_WARN  =  2,
    LOG_LEVEL_INFO  =  3,
    LOG_LEVEL_DEBUG =  4
} tLogLevel;

#define LOG_TEXT_DEFAULT "UNKNOWN"

extern char *LOG_TEXT[];

#define LOG_OUT_STDERR_1         "STDERR"
#define LOG_OUT_STDERR_2         "-"
#define LOG_DATE_FORMAT          "%04d/%02d/%02d %02d:%02d:%02d"
#define LOG_FORMATED_DATE_LENGTH 19

void logInit(const char *filename, tLogLevel level);
void logEnd(void);

void logFatal(char *filename, int lineNumber, char *text, ...);
void logError(char *filename, int lineNumber, char *text, ...);
void logWarn (char *filename, int lineNumber, char *text, ...);
void logInfo (char *filename, int lineNumber, char *text, ...);
void logDebug(char *filename, int lineNumber, char *text, ...);

void _logAssert(char *filename, int line, char *assertion, int test);

#define logAssert(a) _logAssert(__FILE__, __LINE__, #a, a)

#endif
