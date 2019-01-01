#ifndef _LIBMYTHREAD_H_
#define _LIBMYTHREAD_H_

#include <incboolean/libboolean.h>

typedef long mythread_tid_t;

boolean_t mythreadIsMainThread(void);
mythread_tid_t mythreadGetTID(void);

#endif

