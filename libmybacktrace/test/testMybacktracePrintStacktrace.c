#include <stdio.h>
#include <stdlib.h>

#include <libmybacktrace/libmybacktrace.h>

void dummy_function3(int a) {
  mybacktracePrintStacktrace();
}

void dummy_function2(int a) {
  dummy_function3(a + 1);
}

void dummy_function1(void) {
  dummy_function2(1);
}

int main(int argc, char* argv[]) {
  dummy_function1();
  return 0;
}
