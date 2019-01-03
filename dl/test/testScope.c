
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#define LIB_TEST_PATH "libmydltest.so"

#define TYPE_FUNCTION function_t
#define TYPE_FUNCTION_CAST (TYPE_FUNCTION)
typedef void(*TYPE_FUNCTION)();

#define TYPE_VARIABLE t_variable
#define TYPE_VARIABLE_CAST (TYPE_VARIABLE)
typedef int* TYPE_VARIABLE;



#define TYPE_LIB void*

char* variableSymbols[] = {
  "static_int",
  "extern_int",
  "unknow_var",
  NULL
};

char* functionSymbols[] = {
  "static_function",
  "extern_function",
  "unknow_function",
  NULL
};

int main(int argc, char *argv[]) {
  int           idx      = 0;
  TYPE_LIB      lib      = NULL;
  TYPE_FUNCTION f        = NULL;
  TYPE_VARIABLE v        = NULL;

  /* load lib into memory */
  if ((lib = dlopen(LIB_TEST_PATH, RTLD_LAZY)) == NULL) {
    printf("Library load '%s' is impossible : %s\n", LIB_TEST_PATH, dlerror());
    return 1;
  }

  for (idx = 0; variableSymbols[idx] != NULL; idx++) {
    /* resolve symbol  */
    char* varName  = variableSymbols[idx];
    if ((v = TYPE_VARIABLE_CAST dlsym(lib, varName)) == NULL) {
      printf("Variable load '%s' is impossible : %s\n", varName, dlerror());
    } else {
      printf("Variable '%s' value is %d\n", varName, *v);
    }
  }

  for (idx = 0; functionSymbols[idx] != NULL; idx++) {
    /* resolve symbol  */
    char* varName  = variableSymbols[idx];
    char* funcName = functionSymbols[idx];
    if ((f = TYPE_FUNCTION_CAST dlsym(lib, funcName)) == NULL) {
      printf("Function load '%s' is impossible : %s\n", funcName, dlerror());
    } else {
      printf("Function '%s' is at 0x%x\n", funcName, f);
      f();
      printf("After function '%s' call, variable '%s' value is %d\n", funcName, varName, *(TYPE_VARIABLE_CAST dlsym(lib, varName)));
    }
  }

  /* unload lib */
  if (dlclose(lib) != 0) {
    printf("Library unload '%s' is impossible : %s\n", LIB_TEST_PATH, dlerror());
    return 3;
  }

  return 0;
}
