
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int sum(int counter, ...) {
    int result = 0;
    va_list varg;

    int value = 0;

    printf ("Counter = %d\n", counter);

    va_start (varg, counter);
    while (counter != 0) {
        value = va_arg (varg, int);
        printf ("- Value = %d\n", value);

        result = result + value;
        printf ("  - Result  = %d\n", result);

        counter = counter - 1;
        printf ("  - Counter = %d\n", counter);
    };

    va_end (varg);

    printf ("Result  = %d\n", result);
    return result;
}

void strlist (int counter, ...) {
    va_list liste;

    printf ("Counter = %d\n", counter);

    if (counter < 0) {
        return;
    }

    char* value = NULL;

    va_start (liste, counter);
    while (counter > 0) {
        value = va_arg (liste, char*);
        printf ("- Value = \"%s\"\n", value);

        counter--;
        printf ("  - Counter = %d\n", counter);
    }
}

void format(char* fmt, ...) {
    va_list ap;
    int d;
    char c, *s;

    printf("Format = \"%s\"\n", fmt);

    va_start(ap, fmt);
    while (*fmt) {
        switch (*fmt++) {
            case 's': {             /* string */
                s = va_arg(ap, char *);
                printf("- String = \"%s\"\n", s);
                break;
            }
            case 'd': {             /* int */
                d = va_arg(ap, int);
                printf("- Integer = %d\n", d);
                break;
            }
            case 'c': {             /* char */
                /* need a cast here since va_arg only
                  takes fully promoted types */
                c = (char) va_arg(ap, int);
                printf("- Char = '%c'\n", c);
                break;
            }
        }
    }
    va_end(ap);
}

int main (void) {

    char buffer[1000];
    int i;

    memset(buffer, 0, 1000);

    for (i = 0; i < 1000 - 1; i++) {
        buffer[i] = (i % 26) + 'A';
    }

    printf("[main] Sum = %d\n", sum(2, 5, 7));

    printf("\n");

    strlist(4, "Ch1", "Ch2", "Ch3", buffer);

    printf("\n");

    format("sdc", "STR", 1234, 'X');

    return EXIT_SUCCESS;
}
