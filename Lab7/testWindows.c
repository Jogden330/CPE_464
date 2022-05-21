#include "windows.h"
#include "UDP.h"

#define MAXBUF 1400

int readFromStdin(char * buffer);

int main(int argc, char * argv[]){

 serverWindow * window;
 window = window_init(4); 
 int datalen;
 uint32_t seqNum =  0;
 int RR = 0;
 int len;
 char buffer[MAXBUF];
 int i;
 uint8_t pduBuffer[MAXBUF];
 while(1){

    if(isOpen(window)){
       datalen += 20;
       for(i = 0; i < datalen; i++){
           pduBuffer[i] = (rand() %100 )+1;
       }
       len = createPDU(pduBuffer, seqNum++, 3,(uint8_t *) buffer , datalen);
       addPDUtoWindow(window, pduBuffer, len+1);
       printServerWindow_metadata(window);
       window->current++;
              
    } else {
      
       printEntireWindow(window);

       printServerWindow_metadata(window);
       printf("Enter Number to RR: ");
       scanf("%d",&RR);
       fflush(stdin);
       //RR the sequence number 
       processRR(window, RR);
       printEntireWindow(window);

       printServerWindow_metadata(window);
     
    }
    

 }

 return 0;
}
