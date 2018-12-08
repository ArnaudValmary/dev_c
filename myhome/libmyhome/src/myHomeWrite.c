
#include <libmyhome_private.h>

boolean myHomeWrite(myHomeSession_t* pSession, const char* buffer, const int bufferSize) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  if (pSession->socketId < 0) {
    fprintf(stderr, "Function '%s', line %d, invalid socket id (socket id=%d)\n", __func__, __LINE__, pSession->socketId);
    return false;
  }
  if (buffer == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid buffer argument (buffer='%s')\n", __func__, __LINE__, MYHOME_STR_OR_NULL(buffer));
    return false;
  }
  if (bufferSize <= 0) {
    fprintf(stderr, "Function '%s', line %d, invalid buffer size argument (buffer size=%d)\n", __func__, __LINE__, bufferSize);
    return false;
  }
  int n = 0;
  n = send(pSession->socketId, buffer, bufferSize, 0);
  if (n < 0) {
    fprintf(stderr, "Function '%s', line %d, Cannot write socket (errno=%d, message='%s')\n", __func__, __LINE__, errno, strerror(errno));
    return false;
  }
  if (n == 0) {
    fprintf(stderr, "Function '%s', line %d, No character write (errno=%d, message='%s')\n", __func__, __LINE__, errno, strerror(errno));
    return false;
  }
  if (n != bufferSize) {
    fprintf(stderr, "Function '%s', line %d, No all characters written (#characters=%d, expected=%d, errno=%d, message='%s')\n", __func__, __LINE__, n, bufferSize, errno, strerror(errno));
    return false;
  }
  fprintf(stdout, "All characters written (#characters=%d, expected=%d, buffer='%s'\n", n, bufferSize, buffer);
  return true;
}

boolean myHomeSendCommand(myHomeSession_t* pSession, const char* buffer) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  if (! myHomeWrite(pSession, buffer, strlen(buffer))) {
    return false;
  }
  return myHomeReadAck(pSession);
}
