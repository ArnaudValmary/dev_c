#ifndef _LIBBOOLEAN_H_
#define _LIBBOOLEAN_H_

typedef enum {
    FALSE = 0,
    TRUE  = 1
} boolean_t;

#define booleanTrue()  (TRUE)
#define booleanFalse() (FALSE)

#define booleanIsTrue(a)  ((a==FALSE)?(FALSE):(TRUE))
#define booleanIsFalse(a) ((a==FALSE)?(TRUE):(FALSE))

#endif
