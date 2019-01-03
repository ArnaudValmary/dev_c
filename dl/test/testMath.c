
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#define LIB_M_PATH "/lib64/libm.so.6"

#define TYPE_FUNCTION function_t
#define TYPE_FUNCTION_CAST (TYPE_FUNCTION)
typedef double(*TYPE_FUNCTION)(double);

#define TYPE_LIB void*

char* basename(char* filepath) {
  if (filepath == NULL) {
    return NULL;
  } else {
    int idx = 0;
    for(idx = strlen(filepath); idx > 0 && filepath[idx - 1] != '/'; idx--);
    return &filepath[idx];
  }
}

int main(int argc, char *argv[]) {
  int           idx      = 0;
  TYPE_LIB      lib      = NULL;
  TYPE_FUNCTION fMath    = NULL;
  char*         progname = basename(argv[0]);

  /* load libm into memory */
  if ((lib = dlopen(LIB_M_PATH, RTLD_LAZY)) == NULL) {
    printf("Library load '%s' is impossible : %s\n", LIB_M_PATH, dlerror());
    return 1;
  }

  /* resolve symbol  */
  if ((fMath = (TYPE_FUNCTION) dlsym(lib, progname)) == NULL) {
    printf("Function load '%s' is impossible : %s\n", progname, dlerror());
    return 2;
  }

  /* print math function of all arguments: */
  for (idx = 1; idx < argc; idx++) {
    printf("%s(%f) = %f\n", progname, atof(argv[idx]), fMath(atof(argv[idx])));
  }

  /* unload libm */
  if (dlclose(lib) != 0) {
    printf("Unload library '%s' is impossible : %s\n", LIB_M_PATH, dlerror());
    return 3;
  }

  return 0;
}
