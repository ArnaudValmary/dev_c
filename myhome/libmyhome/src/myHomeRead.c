
#include <libmyhome_private.h>

char* myHomeRead(myHomeSession_t* pSession) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  if (pSession->socketId < 0) {
    fprintf(stderr, "Function '%s', line %d, invalid socket id argument (socket id=%d)\n", __func__, __LINE__, pSession->socketId);
    return NULL;
  }

  char* buffer = (char*) malloc(sizeof(char) * LIBMYHOME_BUFFER_ALLOC_SIZE);
  memset(buffer, 0, sizeof(char) * LIBMYHOME_BUFFER_ALLOC_SIZE);
  int n = 0;
  while (true) {
    errno = 0;
    alarm(1);
    n = recv(pSession->socketId, &buffer[strlen(buffer)], sizeof(char) * (LIBMYHOME_BUFFER_ALLOC_SIZE - 1), 0); // No flags !
    alarm(0);
    if (errno == EINTR && n < 0) {
      // Ignore interruption from SIGALRM
      errno = 0;
      n = 0;
    }
    if (errno != 0 || n < 0) {
      fprintf(stderr, "Function '%s', line %d, cannot read socket (n=%d, errno=%d, message='%s', buffer='%s')\n", __func__, __LINE__, n, errno, strerror(errno), MYHOME_STR_OR_NULL(buffer));
      MYHOME_FREE_BUFFER(buffer);
      return NULL;
    }
    if (n == 0) {
      break;
    } else if (n == (sizeof(char) * (LIBMYHOME_BUFFER_ALLOC_SIZE - 1))) {
      fprintf(stdout, "Read buffer max (n=%d, strlen(buffer)=%ld, buffer='%s', realloc=%ld)\n", n, strlen(buffer), buffer, strlen(buffer) + LIBMYHOME_BUFFER_ALLOC_SIZE + 1);
      buffer = realloc(buffer, strlen(buffer) + LIBMYHOME_BUFFER_ALLOC_SIZE + 1);
      memset(&buffer[strlen(buffer) + 1], 0, strlen(buffer) + LIBMYHOME_BUFFER_ALLOC_SIZE + 1);
    } else if (n < (int)((LIBMYHOME_BUFFER_ALLOC_SIZE - 1) * sizeof(char))) {
      fprintf(stdout, "Read all (n=%d, strlen(buffer)=%ld, buffer='%s')\n", n, strlen(buffer), buffer);
      break;
    }
  }
  if (strlen(buffer) == 0) {
    fprintf(stderr, "Function '%s', line %d, no character read (errno=%d, message='%s')\n", __func__, __LINE__, errno, strerror(errno));
    MYHOME_FREE_BUFFER(buffer);
    return NULL;
  }
  fprintf(stdout, "Read end (strlen(buffer)=%ld, buffer='%s')\n", strlen(buffer), buffer);
  return buffer;
}

boolean mownReadResponses(myHomeSession_t* pSession, int* pNbBuffer, char*** pBuffers, boolean flagReturnACK) {
  *pNbBuffer = 0;
  *pBuffers  = NULL;
  char** buffers       = (char**) malloc(sizeof(char*) * 1024);
  int    sizeofBuffers = 1024;
  int    nbBuffer      = 0;
  char*  buffer        = NULL;
  while ((buffer = myHomeRead(pSession)) != NULL) {
    // TODO : Attention to multi-response in one buffer
    if (strcmp(buffer, MYHOME_OWN_ACK) == 0 && ! flagReturnACK) {
      MYHOME_FREE_BUFFER(buffer);
      break;
    }
    if (nbBuffer < sizeofBuffers) {
      buffers[nbBuffer] = buffer;
      nbBuffer++;
    } else {
      buffers = (char**) realloc(buffers, sizeof(char*) *(sizeofBuffers + 1024));
      sizeofBuffers += 1024;
      buffers[nbBuffer] = buffer;
      nbBuffer++;
    }
    if (strcmp(buffer, MYHOME_OWN_ACK) == 0 && flagReturnACK) {
      break;
    }
  }
  *pBuffers  = buffers;
  *pNbBuffer = nbBuffer;
  return true;
}

boolean myHomeReadAck(myHomeSession_t* pSession) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  char** buffers = NULL;
  int   nbBuffers = 0;
  boolean retFlag = false;
  if (mownReadResponses(pSession, &nbBuffers, &buffers, true)) {
    int idx = 0;
    if (nbBuffers == 1 && strcmp(buffers[idx], MYHOME_OWN_ACK) == 0) {
      retFlag = true;
      fprintf(stdout, "ACK\n");
    }
    for (idx = 0; idx < nbBuffers; idx++) {
      fprintf(stdout, "Read buffer #%d = '%s'\n", idx, buffers[idx]);
      MYHOME_FREE_BUFFER(buffers[idx]);
    }
    MYHOME_FREE_BUFFER(buffers);
  }
  return retFlag;
}

boolean myHomeReadStatus(myHomeSession_t* pSession, const char* command, int* pNbBuffer, char*** pBuffers) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  if (! myHomeWrite(pSession, command, strlen(command))) {
    return false;
  }
  return mownReadResponses(pSession, pNbBuffer, pBuffers, false);
}
