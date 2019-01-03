#ifndef _LIBMYLOG_PRIVATE_H_
#define _LIBMYLOG_PRIVATE_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <libmylog/libmylog.h>

extern FILE *_Global_MyLogFile;

extern boolean_t _Global_IsMyLog;
extern boolean_t _Global_IsMyLogFile;

extern mylog_level_t _Global_MyLogLevel;

#endif
