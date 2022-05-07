#include "PDUlab.h"


int recvPDU(int clientSocket, uint8_t * dataBuffer, int bufferLen){
     
    uint8_t MessLen;
    
    if ((MessLen = recv(clientSocket, dataBuffer, 2, MSG_WAITALL)) < 0){
           perror("recv");
           exit(-1);
    }

    
    if (MessLen == 0) {   
       return 0;
    }
   
    
    memcpy(&bufferLen, dataBuffer, 2);

    bufferLen = ntohs(bufferLen) - 2;
    
    if((MessLen = recv(clientSocket, dataBuffer, bufferLen, MSG_WAITALL)) < 0 ){
       perror("rec");
       exit(-1);
    }
    
    if(MessLen > bufferLen){
       perror("buffer Len");
       exit(-1);
    }

    return bufferLen;

}


int sendPDU(int socketNumber, uint8_t * dataBuffer, int lengthOfData){
        
     uint8_t dataWithHeader[lengthOfData + 2];  
     int16_t dataLen = htons(lengthOfData + 2);

     memcpy( dataWithHeader, &dataLen, 2 );
     memcpy( &dataWithHeader[2], dataBuffer, lengthOfData);

     if (send(socketNumber, dataWithHeader, (lengthOfData + 2), MSG_WAITALL) < 0  ){
           perror("send");
           return -1;
      } 

   return lengthOfData;
 

}
