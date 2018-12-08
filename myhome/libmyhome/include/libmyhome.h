#ifndef _LIBMYHOME_H_
#define _LIBMYHOME_H_

#include <stdlib.h> // for free

#define MYHOME_OWN_ACK            "*#*1##"
#define MYHOME_OWN_NACK           "*#*0##"

#define MYHOME_OWN_SESSION_ACTION "*99*9##"
#define MYHOME_OWN_SESSION_EVENT  "*99*1##"

#define MYHOME_PTR_OR_NULL(a) (((a)==NULL)?"_null_":"_not_null_")
#define MYHOME_STR_OR_NULL(a) (((a)==NULL)?"_null_":(a))
#define MYHOME_FREE_BUFFER(a) if(a)free(a);a=NULL

typedef int socket_t;
typedef unsigned long long int myHomePass_t;

#ifndef boolean
typedef enum {false = 0,  true = 1}  boolean;
#endif

typedef enum {off = 0, on     = 1}            myHomeSessionState_t;
typedef enum {no  = 0, action = 1, event = 2} myHomeSessionType_t;

typedef struct {
  myHomeSessionState_t state;
  myHomeSessionType_t  type;
  char*                gatewayIP;
  char*                gatewayPort;
  char*                openPassword;
  socket_t             socketId;
  boolean              flagVerbose;
} myHomeSession_t;

myHomeSession_t* myHomeInitSession(void);
myHomeSession_t* myHomeInitSession__withParams(char* gatewayIP, char* gatewayPort, char* openPassword);
boolean          myHomeStartEventSession(myHomeSession_t* pSession);
boolean          myHomeStartActionSession(myHomeSession_t* pSession);
void             myHomeCloseSession(myHomeSession_t* pSession);
boolean          myHomeReadStatus(myHomeSession_t* pSession, const char* command, int* pNbBuffer, char*** pBuffers);
boolean          myHomeSendCommand(myHomeSession_t* pSession, const char* buffer);

#endif
