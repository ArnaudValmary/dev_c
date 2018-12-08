
#include <libmyhome_private.h>

void myHomeSignalHandler(int signum, siginfo_t* siginfo, void* voidptr) {
  switch (signum) {
    case SIGALRM :
    default : {
      fprintf(stdout, "Receive signal (signal number=%d)\n", signum);
    }
  }
}

boolean myHomeSignalConfig(void) {
  static int flagInit = 0;
  if (flagInit == 1) {
    fprintf(stdout, "***** Signal already initialized *****\n");
    return true;
  }
  flagInit = 1;
  int i = 0;
  int signalList[] = {
    SIGALRM,
    0
  };
  int retCode = 0;
  struct sigaction action;

  action.sa_sigaction = &myHomeSignalHandler;
  action.sa_flags     = SA_SIGINFO;

  for (i = 0; signalList[i] != 0; i++) {
    retCode = 0;
    if (signalList[i] == SIGCHLD) {
      struct sigaction actionTmp = action;
      actionTmp.sa_flags = action.sa_flags | SA_NOCLDSTOP | SA_NOCLDWAIT;
      retCode = sigaction(signalList[i], &actionTmp, NULL);
    } else {
      retCode = sigaction(signalList[i], &action, NULL);
    }
    if (retCode < 0) {
      fprintf(stderr, "Function '%s', line %d, cannot initialize signal (signal number=%d)\n", __func__, __LINE__, signalList[i]);
      return false;
    }
  }
  return true;
}
