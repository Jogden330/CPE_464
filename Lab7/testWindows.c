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

 uint8_t pduBuffer[MAXBUF];
 while(1){

    if(isOpen(window)){
       datalen = readFromStdin(buffer);
       len = createPDU(pduBuffer, seqNum++, 3,(uint8_t *) buffer , datalen);
       addPDUtoWindow(window, pduBuffer, len+1);
       printServerWindow_metadata(window);
       window->current++;
              
    } else {
      
       printEntireWindow(window);

       printServerWindow_metadata(window);
       printf("Enter Number to RR: ");
       scanf("%d",&RR);
       //RR the sequence number 
       processRR(window, RR);
       printEntireWindow(window);

       printServerWindow_metadata(window);
     
    }
    

 }

 return 0;
}
int readFromStdin(char * buffer)
{
	char aChar = 0;
	int inputLen = 0;        
	
	// Important you don't input more characters than you have space 
	buffer[0] = '\0';
	printf("Enter data: ");
	while (inputLen < (MAXBUF - 1) && aChar != '\n')
	{
		aChar = getchar();
		if (aChar != '\n')
		{
			buffer[inputLen] = aChar;
			inputLen++;
		}
	}
	
	// Null terminate the string
	buffer[inputLen] = '\0';
	inputLen++;
	
	return inputLen;
}
