#include "comm.h"
#include "PDUlab.h"

int sendCommand(int socket, char * buf, uint8_t flag){                                                                 
  memcpy(buf, &flag, 1);                                                                                                    
  return sendTo(socket, (uint8_t *) buf, 1);                          
} 

int sendTo(int socketNum, uint8_t * sendBuf, int sendLen)
{

   int sent = 0;      //actual amount of data sent/* get the data and send it   */    
   sent = sendPDU(socketNum, sendBuf, sendLen);

  if (sent < 0) {
      perror("send call");
      exit(-1);
  }


  if (sent == 0){
    return 0;
  }
  return 1;

}

void sendHandle(int socketNum, char * buf, uint8_t flag, char * handle){

  uint8_t handLen = strlen(handle);

  memcpy(buf, &flag, 1);
  memcpy(&buf[1], &handLen, 1);
  memcpy(&buf[2], handle, handLen);

  sendTo(socketNum, (uint8_t *) buf, handLen + 2);

}

int addHandleToMess(uint8_t * buf, char * handle){

  uint8_t handLen = strlen(handle);

 
  memcpy(&buf[0], &handLen, 1);
  memcpy(&buf[1], handle, handLen);
  return handLen + 1;

}

int prossesHandle(uint8_t * buf, char * handle){
     
     uint8_t handleLen;
 
     memset(handle, '\0', MAXHANDLE);
     memcpy(&handleLen, &buf[0], 1);
     memcpy(handle, &buf[1], handleLen);
 
     return handleLen;
}
