#include "windows.h"


clientWindow * window_init(int windowsize ){
     
    int i;
    Data ** data = malloc(sizeof(Data*)*windowsize);
    clientWindow * window= malloc(sizeof(clientWindow));
   
    window->upper = windowsize;
    window->lower = 0;
    window->current = 0;
    window->windowsize = windowsize;
    window->PDUs = data;
 
    for(i = 0; i < window->windowsize; i++){
	data[i] = NULL;
    }
    return window;

}

void printServerWindow_metadata(clientWindow * window){
    printf("windowsize %d, current %d, lower %d, upper %d\n", window->windowsize, window->current, window->lower, window->upper);

}

void printEntireWindow(clientWindow * window){

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

void addToWindow(clientWindow * window, uint8_t * pduBuff, int size, uint32_t seqNum){

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

int isOpen(clientWindow * window){
  if( window->upper >  window->current){
	return 1;
  } 
  return 0;

}

int isEmpty(clientWindow * window){
  if( window->lower ==  window->current)
	return 1;
  return 0;

}

int findPDU(clientWindow * window, uint8_t * pduBuff, uint32_t seqNum){
	
	Data * data;
	int index = 0;        
        index = seqNum % window->windowsize; 

	 
	data =  window->PDUs[index];
        if(data == NULL)
		return -1;

	memcpy(pduBuff, data->pdu, data->size);

	return data->size;


}

void processRR(clientWindow * window, int RR){

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
