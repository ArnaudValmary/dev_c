
#include <libmybacktrace/libmybacktrace.h>

#include <execinfo.h>

#include <stdio.h>
#include <stdlib.h>

void mybacktracePrintStacktrace(void) {
  void   *array[MYBACKTRACE_DEPTH_MAX];
  size_t size      = 0;
  char   **strings = NULL;
  size_t i         = 0;

  size    = backtrace(array, MYBACKTRACE_DEPTH_MAX);
  strings = backtrace_symbols(array, size);

  printf("Backtrace depth: %zd\n", size);

  for (i = 0; i < size; i++) {
    printf("- %s\n", strings[i]);
  }

  free(strings);
}
