
#include <incmpfr/incmpfr.h>

int main (void) {

    printf("Simple test\n");

    {
        WITH_MPFR(n, 100, "1.234567", 10, MPFR_RNDN) {
            mpfr_printf(" %.10R*f\n", MPFR_RNDN, n);
            putchar(' ');
            mpfr_out_str(stdout, 10, 10, n, MPFR_RNDN);
            putchar('\n');
        }
        END_WIH_MPFR(n);
    }

    printf("Add operator\n");

    {
        WITH_MPFR(opr, 100, "0", 10, MPFR_RNDN);
        WITH_MPFR(op1, 100, "15", 10, MPFR_RNDN);
        WITH_MPFR(op2, 100, "75", 10, MPFR_RNDN);

        printf(" Before add:\n");
        mpfr_printf("  op1 = %.10R*f\n", MPFR_RNDN, op1);
        mpfr_printf("  op2 = %.10R*f\n", MPFR_RNDN, op2);
        mpfr_printf("  opr = %.10R*f\n", MPFR_RNDN, opr);
        mpfr_add(opr, op1, op2, MPFR_RNDN);
        printf(" After add:\n");
        mpfr_printf("  op1 = %.10R*f\n", MPFR_RNDN, op1);
        mpfr_printf("  op2 = %.10R*f\n", MPFR_RNDN, op2);
        mpfr_printf("  opr = %.10R*f\n", MPFR_RNDN, opr);

        END_WIH_MPFR(op2);
        END_WIH_MPFR(op1);
        END_WIH_MPFR(opr);
    }

    printf("My add operator\n");

    {
        WITH_MPFR(op1, 100, "15", 10, MPFR_RNDN);
        WITH_MPFR(op2, 100, "75", 10, MPFR_RNDN);

        printf(" Before add:\n");
        mpfr_printf("  op1 = %.10R*f\n", MPFR_RNDN, op1);
        mpfr_printf("  op2 = %.10R*f\n", MPFR_RNDN, op2);
        MPFR_ADD(op1, op2, MPFR_RNDN);
        printf(" After add:\n");
        mpfr_printf("  op1 = %.10R*f\n", MPFR_RNDN, op1);
        mpfr_printf("  op2 = %.10R*f\n", MPFR_RNDN, op2);

        END_WIH_MPFR(op2);
        END_WIH_MPFR(op1);
    }

    mpfr_free_cache ();
    return 0;
}
