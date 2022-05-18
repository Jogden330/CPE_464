#include "windows.h"


serverWindow * window_init(int windowsize ){
     

    Data * data = malloc(sizeof(Data)*windowsize);
    serverWindow * window= malloc(sizeof(serverWindow));
   
    window->upper = windowsize;
    window->lower = 0;
    window->current = 0;
    window->windowsize = windowsize;
    window->PDUs = data;
 
    return window;

}

void printServerWindow_metadata(serverWindow * window){
    prinf("windowsize %d, current %d, lower %d, upper %d\n", window->windowsize, window->current, window->lower, window->upper);

}

void printEntireWindow(serverWindow * window){

 int i;
 
 Data * data;
 printf("windowsize %d\n", window->windowsize);
 for(i = 0; i < window->windowsize; i++){

    data = window->PDUs[i];
    printf("\t%d sequenceNumber: %d pduSize: %d\n", i, data->seqNum, data->size );   
   
 } 

}

void addPDUtoWindow(serverWindow * window, uint8_t * pduBuff, int size){

      uint32_t seqNum;
      int index;
      Data * data;
  
      memcpy(&seqNum, pduBuffer, sizeof(uint32_t));

      seqNum = ntohl(seqNum);

      index = seqNum % window->windowsize;     
      data = window->PDUs[index];
      data->seqNum = seqNum;
      data->size = size;
      data->index = index;
      data->valflag = 1;
      data->pdu = pduBuffer;    
     

}
