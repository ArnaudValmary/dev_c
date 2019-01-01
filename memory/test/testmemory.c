
#include <stdio.h>
#include <stdlib.h>

#ifdef __LP64__
#define SIZE_OF_FORMAT "lu"
#define N_BITS 64
#else
#define SIZE_OF_FORMAT "u"
#define N_BITS 32
#endif

int main(void) {

  printf("Memory sizes, v1.1 %s %s\n", __DATE__, __TIME__);
  printf("#Bits: %u\n", N_BITS);

  printf("\n");
  printf("Integers:\n");
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "char",                      sizeof(char));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "unsigned char",             sizeof(unsigned char));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "short",                     sizeof(short));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "unsigned short",            sizeof(unsigned short));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "short int",                 sizeof(short int));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "short unsigned int",        sizeof(short unsigned int));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "int",                       sizeof(int));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "long int",                  sizeof(long int));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "long long int",             sizeof(long long int));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "unsigned int",              sizeof(unsigned int));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "long unsigned int",         sizeof(long unsigned int));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "long long unsigned int",    sizeof(long long unsigned int));

  printf("\n");
  printf("Floats:\n");
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "float",                     sizeof(float));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "double",                    sizeof(double));
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "long double",               sizeof(long double));

  printf("\n");
  printf("Pointer:\n");
  printf(" - %30s : %3" SIZE_OF_FORMAT " byte(s)\n", "void*",                     sizeof(void*));

  return EXIT_SUCCESS;
}
