
// TODO : Add event session mode with read infinity loop

#define MYHOME_PROG_NAME "testmyhome"
#define MYHOME_PROG_VERSION "0.0.1"
#define MYHOME_PROG_DATE "2016-11-23"
#define MYHOME_PROG_COPYRIGHT "VALMARY Arnaud"
#define MYHOME_PROG_LICENCE "This program is distributed under GPL v3 without any guaranties"

#include <stdio.h>  // for *printf
#include <string.h> // for memset, strcpy
#include <unistd.h> // for sleep

#include <libmyhome/libmyhome.h>

#define MYHOME_DEFAULT_GATEWAY_PORT "20000"
void printHelp(void) {
  fprintf(stdout, "%s v%s %s (c) %s\n%s\n", MYHOME_PROG_NAME, MYHOME_PROG_VERSION, MYHOME_PROG_DATE, MYHOME_PROG_COPYRIGHT, MYHOME_PROG_LICENCE);
  fprintf(stdout, "%s [options]\n", MYHOME_PROG_NAME);
  fprintf(stdout, "Options:\n");
  fprintf(stdout, "  Standard:\n");
  fprintf(stdout, "    --help | -h                      : Print this help\n");
  fprintf(stdout, "    --version | -V                   : Print version\n");
  fprintf(stdout, "    --verbose | -v                   : Verbose mode\n");
  fprintf(stdout, "  Connection:\n");
  fprintf(stdout, "    --gatewayIP | -i ip              : Use IP address\n");
  fprintf(stdout, "    --gatewayPort | -P tcpPort       : Use TCP port (default is %s)\n", MYHOME_DEFAULT_GATEWAY_PORT);
  fprintf(stdout, "  Authentification:\n");
  fprintf(stdout, "    --openPassword | -p openPassword : Gateway open password\n");
  fprintf(stdout, "  Command:\n");
  fprintf(stdout, "    --action | -a action             : Send action command and expect ACK\n");
  fprintf(stdout, "  Status:\n");
  fprintf(stdout, "    --status | -s status             : Send status command and expect ACK\n");
  fprintf(stdout, "\n");
}

void printVersion(void) {
  fprintf(stdout, "%s v%s %s\n", MYHOME_PROG_NAME, MYHOME_PROG_VERSION, MYHOME_PROG_DATE);
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    printHelp();
    return 0;
  }
  char* gatewayIP     = NULL;
  char* gatewayPort   = NULL;
  char* openPassword  = NULL;
  char* actionCommand = NULL;
  char* statusCommand = NULL;
  boolean flagVerbose = false;
  int nbArgs          = argc;
  int idxArg          = 1;
  char* arg           = NULL;
  while (idxArg < nbArgs) {
    arg = argv[idxArg];
    idxArg++;
    if (arg == NULL) {
      // Arg! :-/
      fprintf(stderr, "Function '%s', line %d, Unexpected error while reading arguments (idxArg=%d, arg='%s')\n", __func__, __LINE__, idxArg, MYHOME_STR_OR_NULL(arg));
      return 1;
    }
    if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
      printHelp();
      return 0;
    } else if (strcmp(arg, "--version") == 0 || strcmp(arg, "-V") == 0) {
      printVersion();
      return 0;
    } else if (strcmp(arg, "--verbose") == 0 || strcmp(arg, "-v") == 0) {
      flagVerbose = true;
    } else if (strcmp(arg, "--gatewayIP") == 0 || strcmp(arg, "-i") == 0) {
      if (idxArg >= nbArgs) {
        fprintf(stderr, "Option '%s' need an argument\n", MYHOME_STR_OR_NULL(arg));
        return 2;
      }
      gatewayIP = argv[idxArg];
      idxArg++;
    } else if (strcmp(arg, "--gatewayPort") == 0 || strcmp(arg, "-P") == 0) {
      if (idxArg >= nbArgs) {
        fprintf(stderr, "Option '%s' need an argument\n", MYHOME_STR_OR_NULL(arg));
        return 2;
      }
      gatewayPort = argv[idxArg];
      idxArg++;
    } else if (strcmp(arg, "--openPassword") == 0 || strcmp(arg, "-p") == 0) {
      if (idxArg >= nbArgs) {
        fprintf(stderr, "Option '%s' need an argument\n", MYHOME_STR_OR_NULL(arg));
        return 2;
      }
      openPassword = argv[idxArg];
      idxArg++;
    } else if (strcmp(arg, "--action") == 0 || strcmp(arg, "-a") == 0) {
      if (idxArg >= nbArgs) {
        fprintf(stderr, "Option '%s' need an argument\n", MYHOME_STR_OR_NULL(arg));
        return 2;
      }
      actionCommand = argv[idxArg];
      idxArg++;
    } else if (strcmp(arg, "--status") == 0 || strcmp(arg, "-s") == 0) {
      if (idxArg >= nbArgs) {
        fprintf(stderr, "Option '%s' need an argument\n", MYHOME_STR_OR_NULL(arg));
        return 2;
      }
      statusCommand = argv[idxArg];
      idxArg++;
    } else {
      fprintf(stderr, "Unknown parameter '%s'\n", MYHOME_STR_OR_NULL(arg));
      return 3;
    }
  }
  if (! gatewayIP) {
      fprintf(stderr, "You must use '--gatewayIP' option\n");
      return 3;
  }
  if (! gatewayPort) {
    gatewayPort = MYHOME_DEFAULT_GATEWAY_PORT;
  }
  if (! openPassword) {
      fprintf(stderr, "You must use '--openPassword' option\n");
      return 3;
  }
  if (! actionCommand && ! statusCommand) {
      fprintf(stderr, "You must use '--action' and/or '--status' option\n");
      return 3;
  }

  // Go!
  myHomeSession_t* pSession = myHomeInitSession__withParams(gatewayIP, gatewayPort, openPassword);
  if (! pSession) {
    fprintf(stderr, "Cannot initialize session\n");
    return 10;
  }
  if (! myHomeStartActionSession(pSession)) {
    myHomeCloseSession(pSession);
    return 12;
  }
  if (actionCommand) {
    if(! myHomeSendCommand(pSession, actionCommand)) {
      myHomeCloseSession(pSession);
      return 13;
    }
  }
  if (statusCommand) {
    char** buffers = NULL;
    int   nbBuffers = 0;
    if (myHomeReadStatus(pSession, statusCommand, &nbBuffers, &buffers)) {
      int idx = 0;
      for (idx = 0; idx < nbBuffers; idx++) {
        fprintf(stdout, "Status buffer #%d = '%s'\n", idx, buffers[idx]);
        MYHOME_FREE_BUFFER(buffers[idx]);
      }
      MYHOME_FREE_BUFFER(buffers);
    }
  }
  myHomeCloseSession(pSession);

  return 0;
}
