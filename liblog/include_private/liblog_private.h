#ifndef _LIBLOG_PRIVATE_H_
#define _LIBLOG_PRIVATE_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <liblog/liblog.h>

extern FILE *_Global_LogFile;

extern boolean_t _Global_IsLog;
extern boolean_t _Global_IsLogFile;

extern tLogLevel _Global_LogLevel;

#endif
