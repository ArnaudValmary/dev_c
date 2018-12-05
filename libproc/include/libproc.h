/**
 * @file
 * You find here all functions for reading '/proc/*' files
 */
#ifndef __LIBPROC_H_
#define __LIBPROC_H_

/** Return the current process program filename
 * @return the current process program filename (a static address with a maximum of 1023 characters)
 */
char* procGetProcessName(void);

#endif