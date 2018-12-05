#ifndef __INCMPFR_H_
#define __INCMPFR_H_

/**
 * For MPFR API (mpfr_t type & MPFR functions)
 */
#include <stdio.h>
#include <mpfr.h>

/**
 * n = variable name
 * p = precision
 * v = value
 * b = base
 * r = rounding mode
 */
#define WITH_MPFR(n, p, v, b, r) \
    do { \
        mpfr_t n; \
        { \
            mpfr_init2(n, p); \
            mpfr_set_str(n, v, b, r); \
            { \

/**
 * n = variable name (necessary from clearing)
 */
#define END_WIH_MPFR(n) \
            } \
        } \
        mpfr_clear(n); \
    } while(0) \

/**
 * op1 = operand #1
 * op2 = operand #2
 * r = rounding mode
 */
#define MPFR_ADD(op1, op2, r) \
    { \
        mpfr_t add_op1_op2_tmp; \
        mpfr_init2(add_op1_op2_tmp, mpfr_get_prec(op1)); \
        mpfr_add(add_op1_op2_tmp, op1, op2, r); \
        mpfr_set(op1, add_op1_op2_tmp, r); \
        mpfr_clear(add_op1_op2_tmp); \
    } \

#endif
