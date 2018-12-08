
#include <libmyhome_private.h>

boolean myHomeStartSession(myHomeSession_t* pSession) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  if (pSession->openPassword == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid open password (open password='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession->openPassword));
    return false;
  }
  char* sessionType = NULL;
  switch (pSession->type) {
    case event : {
      sessionType = MYHOME_OWN_SESSION_EVENT;
      break;
    }
    case action : {
      sessionType = MYHOME_OWN_SESSION_ACTION;
      break;
    }
    case no : {
      fprintf(stderr, "Function '%s', line %d, session type not initialized (type=%d)\n", __func__, __LINE__, pSession->type);
      return false;
    }
    default : {
      fprintf(stderr, "Function '%s', line %d, unknown session type (type=%d)\n", __func__, __LINE__, pSession->type);
      return false;
    }
  }
  if (! myHomeConnect(pSession)) {
    fprintf(stderr, "No connection approved\n");
    return false;
  }
  if (! myHomeWrite(pSession, sessionType, strlen(MYHOME_OWN_SESSION_ACTION))) {
    return false;
  }
  char* buffer = myHomeRead(pSession);
  char nonce[100];
  memset(nonce, 0, 100);
  char* dieze = strstr(buffer, "#");
  if (dieze == NULL) {
    fprintf(stderr, "Function '%s', line %d, no '#' character found in response (buffer='%s')\n", __func__, __LINE__, buffer);
    MYHOME_FREE_BUFFER(buffer);
    return false;
  }
  char* diezedieze = strstr(buffer, "##");
  if (diezedieze == NULL) {
    fprintf(stderr, "Function '%s', line %d, no '##' sequence found in response (buffer='%s')\n", __func__, __LINE__, buffer);
    MYHOME_FREE_BUFFER(buffer);
    return false;
  }
  int start = dieze - buffer;
  if (start <= 0) {
    fprintf(stderr, "Function '%s', line %d, sequence '##' is before '#' character (buffer='%s')\n", __func__, __LINE__, buffer);
  }
  int len = diezedieze - dieze - 1;
  strncpy(nonce, &buffer[start + 1], len);
  fprintf(stdout, "Nonce is '%s'\n", nonce);
  myHomePass_t myPass = myHomePass(pSession->openPassword, nonce);
  fprintf(stdout, "Encoded password is '%lld'\n", myPass);
  char message[100];
  memset(message, 0, 100);
  sprintf(message, "*#%lld##", myPass);
  MYHOME_FREE_BUFFER(buffer);
  return myHomeSendCommand(pSession, message);
}

void myHomeCloseSession(myHomeSession_t* pSession) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return;
  }
  if (pSession->socketId > 0) {
    close(pSession->socketId);
  }
  pSession->socketId = -1;
  pSession->type     = no;
  fprintf(stdout, "Session closed!\n");
}

boolean myHomeStartEventSession(myHomeSession_t* pSession) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  pSession->type = event;
  return myHomeStartSession(pSession);
}

boolean myHomeStartActionSession(myHomeSession_t* pSession) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  pSession->type = action;
  return myHomeStartSession(pSession);
}

myHomeSession_t* myHomeInitSession(void) {
  if (! myHomeSignalConfig()) {
    return NULL;
  }
  myHomeSession_t* pSession = (myHomeSession_t*) malloc(sizeof(myHomeSession_t));
  if (! pSession) {
    fprintf(stderr, "Function '%s', line %d, cannot allocate memory for session\n", __func__, __LINE__);
    return NULL;
  }
  memset(pSession, 0, sizeof(myHomeSession_t));
  pSession->socketId = -1;
  pSession->type     = no;
  fprintf(stdout, "Session initialized\n");
  return pSession;
}

myHomeSession_t* myHomeInitSession__withParams(char* gatewayIP, char* gatewayPort, char* openPassword) {
  myHomeSession_t* pSession = myHomeInitSession();
  if (! pSession) {
    return NULL;
  }
  pSession->gatewayIP    = gatewayIP;
  pSession->gatewayPort  = gatewayPort;
  pSession->openPassword = openPassword;
  return pSession;
}
