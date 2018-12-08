
#include <libmyhome_private.h>

boolean myHomeConnect(myHomeSession_t* pSession) {
  if (pSession == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid session argument (session='%s')\n", __func__, __LINE__, MYHOME_PTR_OR_NULL(pSession));
    return false;
  }
  if (pSession->gatewayIP == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid IP (gateway IP='%s')\n", __func__, __LINE__, MYHOME_STR_OR_NULL(pSession->gatewayIP));
    return false;
  }
  if (pSession->gatewayPort == NULL) {
    fprintf(stderr, "Function '%s', line %d, invalid port (gateway port='%s')\n", __func__, __LINE__, MYHOME_STR_OR_NULL(pSession->gatewayPort));
    return false;
  }

  // Convert TCP port string to integer value
  long port = atol(pSession->gatewayPort);
  if (port == 0) {
    fprintf(stderr, "Function '%s', line %d, cannot use port number (port = '%s')\n", __func__, __LINE__, pSession->gatewayPort);
    return false;
  }

  // Make socket structure
  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));

  server.sin_family      = AF_INET;
  server.sin_port        = htons(port);
  server.sin_addr.s_addr = inet_addr(pSession->gatewayIP);

  // Create socket
  socket_t socketId = socket(AF_INET, SOCK_STREAM, 0);
  if (socketId < 0) {
    fprintf(stderr, "Function '%s', line %d, cannot create socket (errno=%d, message='%s')\n", __func__, __LINE__, errno, strerror(errno));
    return false;
  }

  pSession->socketId = socketId;

  // Connection
  if (connect(pSession->socketId, (struct sockaddr *)&server, sizeof(server)) < 0) {
    fprintf(stderr, "Function '%s', line %d, cannot connect (errno=%d, message='%s')\n", __func__, __LINE__, errno, strerror(errno));
    myHomeCloseSession(pSession);
    return false;
  }

  fprintf(stdout, "Connected (socket id=%d)\n", pSession->socketId);

  // Read response to the connection
  if (! myHomeReadAck(pSession)) {
    myHomeCloseSession(pSession);
  }

  // Yes !
  return true;
}
