#include "windows.h"


serverWindow * window_init(int windowsize ){
     

    Data ** data = malloc(sizeof(Data*)*windowsize);
    serverWindow * window= malloc(sizeof(serverWindow));
   
    window->upper = windowsize;
    window->lower = 0;
    window->current = 0;
    window->windowsize = windowsize;
    window->PDUs = data;
 
    return window;

}

void printServerWindow_metadata(serverWindow * window){
    printf("windowsize %d, current %d, lower %d, upper %d\n", window->windowsize, window->current, window->lower, window->upper);

}

void printEntireWindow(serverWindow * window){

 int i;
 
 Data * data;
 printf("windowsize %d\n", window->windowsize);
 for(i = 0; i < window->windowsize; i++){

    data = window->PDUs[i];
    if(data != NULL){
  	  printf("\t%d sequenceNumber: %d pduSize: %d\n", i, data->seqNum, data->size );   
   } else {
           printf("\t%d not valid\n",i);
   }
 } 

}

void addPDUtoWindow(serverWindow * window, uint8_t * pduBuff, int size, uint32_t seqNum){

      int index;
      Data * data;
      data = malloc(sizeof(Data));
 
      index = seqNum % window->windowsize; 
      data->seqNum = seqNum;
      data->size = size;
      data->index = index;
      data->pdu = pduBuff;    
     
      window->PDUs[index] = data;
}

int isOpen(serverWindow * window){
  if( window->upper >  window->current){
	return 1;
  } 
  return 0;

}

Data * findPDU(serverWindow * window, uint32_t seqNum){
         
	int index = 0;        
        index = seqNum % window->windowsize; 

	return  window->PDUs[index];


}

void processRR(serverWindow * window, int RR){

        int i;
        for(i = 0; i <window->windowsize; i++){
             if(window->PDUs[i]->seqNum < RR){
        	free(window->PDUs[i]);
       	        window->PDUs[i] = NULL;
             }

        }
        if(window->lower < RR){
            window->lower = RR;
    	    window->upper = window->lower + window->windowsize;
        }
}
