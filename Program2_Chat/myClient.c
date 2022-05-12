/******************************************************************************
* myClient.c
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "networks.h"
#include "PDUlab.h"
#include "pollLib.h"
#include "ctype.h"
#include "comm.h"

#define MAXBUF 1400
#define DEBUG_FLAG 1
#define LOCAL 0
#define SERVER 1

int  readFromStdin(char * buffer);
void checkArgs(int argc, char * argv[]);
int  recvFromServer(int serverSocket, uint8_t * buf);
int  clientControl(int socketNum, char * cHandle);
int  parseString(char * buf, int socket, char * handle, int messLen);
int  processStdin(int socketHumber, char *  handle);
int  recvConf(int socketNum, char * buf, char * handle);
int  setupConnect(int socketNum, char * handle);
void readNumLen(uint8_t * buf);
int  getallHandles(int socket);
void  messageSend(int socket,char * buf, uint8_t * messbuf, char * clienthandle);
void badHandle(uint8_t * buf);
void readMessage(char * buf,int len);
void messcopy(char * buf, char * mess, int len);
void brodcast(int socket,char * buf, uint8_t * messbuf, char * handle);
void readBroadCast(char * buf,int len);

int main(int argc, char * argv[])
{
        char cHandle[MAXHANDLE];         
	int socketNum = 0;         //socket descriptor
        checkArgs(argc, argv);

	/* set up the TCP Client socket  */
        
        strcpy(cHandle, argv[1]);
        printf("handle is %s\n", cHandle);
	socketNum = tcpClientSetup(argv[2], argv[3], DEBUG_FLAG);
        if(setupConnect(socketNum, cHandle)){        

            do{
                printf("$: ");
                fflush(stdout);


                if(clientControl(socketNum, cHandle) == 0){
                    break;
                }            
            }while(1);
        }
	close(socketNum);
	
	return 0;
}



int parseString(char * buf, int socket, char * handle, int messLen){

  const char v = '%';
  char control;
  char verify;
  uint8_t messbuf[MAXBUF];

  verify = buf[0];
  control = tolower(buf[1]); 

  if(verify ==  v){

     switch(control){
        case 'e':
            
              sendCommand(socket, buf, 8);

             break; 
         case 'l':
              
              sendCommand(socket, buf, 10);
              return getallHandles(socket);      
             break;
          case 'm':

              messageSend(socket, &buf[3], messbuf, handle);
          break;   
          case 'b':
              brodcast(socket, &buf[3],messbuf, handle);     
          break;
       default:
              printf("\nInvalid Command\n");       
            break;
    }
    
    return 1;
  }
  printf("\nInvalad Command Format\n");
  return 1; 
}

void messcopy(char * buf, char * mess, int len){
      memcpy(buf, mess, len);
}

void brodcast(int socket,char * buf, uint8_t * messbuf, char * handle){

  uint8_t flag = 4;
  int offset = 0;
  char * token;
  char * rest = buf;
  int messLen;
  int sent = 0;

  memcpy(&messbuf[offset++], &flag, 1);
  offset +=  addHandleToMess( &messbuf[offset], handle);

  token = strtok_r(rest, "", &rest);
  if(token == NULL){

          sendTo(socket, messbuf, offset);
          return;
  }
  messLen =  strlen(token);
  while((messLen-sent) > 0){
       if((messLen - sent) < 200){   
          messcopy((char *)&messbuf[offset], &token[sent], messLen);
          sendTo(socket,  messbuf, offset+messLen );
       } else {
          messcopy((char *)&messbuf[offset], &token[sent], 200);
          sendTo(socket, messbuf, offset + 200 );
       }
      sent = sent + 200;
  }
} 
    
void  messageSend(int socket,char * buf, uint8_t * messbuf, char * clienthandle){
  
  uint8_t flag = 5;
  uint8_t numSend;
  int i;
  char * token;
  char * rest = buf;
  int offset = 0;
  int sent = 0;
  int messLen;
  char *ptr;

  if( (token = strtok_r(rest, " ", &rest))==NULL){    
    printf("\nInvalad Command Format\n");
    return;
  }

  memcpy(&messbuf[offset++], &flag, 1);
  offset +=  addHandleToMess( &messbuf[offset], clienthandle);

 
  numSend = strtol(&token[0],&ptr, 10);
  
  memcpy(&messbuf[offset++], &numSend, 1);
  
  if(numSend > 9 || numSend < 1){
     printf("invalid number of clients formating\n");
     return;  
  }
  
  for(i = 0; i < numSend; i++){
       if(( token = strtok_r(rest, " ", &rest)) == NULL){
           printf("\nInvalad Command Format\n");
           return;        
       }
       
        if(strlen(token) > MAXHANDLE - 1){
          printf("Invalid handle, handle longer then 100 characters: %s\n", token);
          return; 
       } else {
           offset += addHandleToMess( &messbuf[offset], token);
       }
  }    
 
  token = strtok_r(rest, "", &rest);
  if(token == NULL){

          sendTo(socket, messbuf, offset);
          return;
  }

  messLen =  strlen(token);
  while((messLen-sent) > 0){
       if((messLen - sent) < 200){   
          messcopy((char *)&messbuf[offset], &token[sent], messLen);
          sendTo(socket,  messbuf, offset+messLen );
       } else {
          messcopy((char *)&messbuf[offset], &token[sent], 200);
          sendTo(socket, messbuf, offset + 200 );
       }
      sent = sent + 200;
  }
}


int getallHandles(int socket){

    
    uint8_t buf[MAXBUF];
    int retVal= 0;
    while(retVal != -2){
       retVal = recvFromServer(socket, buf);
    }
    return retVal;
}

int setupConnect(int socketNum, char * handle){
        
    char buf[1400];

    setupPollSet();
    addToPollSet(socketNum);
    addToPollSet(STDIN_FILENO);
    sendHandle(socketNum, buf, 1, handle);
    return  recvConf(socketNum, buf, handle);
        
}

int recvConf(int socketNum, char * buf, char * handle){
    
    recvFromServer(socketNum, (uint8_t *) buf);

    if(buf[0] == 3){
          printf("Handle Already in use: %s\n", handle);
          return 0;  
    } 
    if(buf[0] == 2){
          return 1;
    }

   return 0;
}

int clientControl(int socketNum, char * cHandle){
     
    uint8_t buf[MAXBUF];
    int socket = pollCall(-1);
    if(socket == STDIN_FILENO){
       if(processStdin(socketNum,cHandle) == 0){
          return 0;
       }
    } else {
        return recvFromServer(socketNum, buf);
    }
    return 1;

}

int processStdin(int socketNum, char * cHandle){

    uint8_t sendBuf[MAXBUF];   //data buffer
    int messLen = 0;        //amount of data to send
      
    messLen = readFromStdin((char *) sendBuf);

        
    return  parseString((char *)sendBuf, socketNum, cHandle, messLen);


}



int readFromStdin(char * buffer)
{
	char aChar = 0;
	int inputLen = 0;        
	
	// Important you don't input more characters than you have space 
	buffer[0] = '\0';
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

void checkArgs(int argc, char * argv[])
{
        int isNum;
        char *ptr;
	/* check command line arguments  */
	if (argc != 4)
	{
		printf("usage: %s handle host-name port-number \n", argv[0]);
		exit(1);
	}
        if(strlen(argv[1]) > MAXHANDLE - 1){
          printf("Invalid handle, handle longer then 100 characters: %s\n", argv[1]);
          exit(1);
       }

       isNum = strtol(&argv[1][0],&ptr, 10);
       if(isNum > 0 && isNum < 10){
          printf("Invalid handle, handle starts with a number\n");
          exit(1);
       }
}

int recvFromServer(int serverSocket, uint8_t *  buf)
{
  int messageLen = 0;
  uint8_t flag;
  char handle[MAXHANDLE];

  memset(buf, 0, MAXBUF); //resets the buffer
  //now get the data from the client_socket
  /* struct pseudoheader *header; */
  if ((messageLen = recvPDU(serverSocket, buf, MAXBUF)) < 0)
    {
      perror("recv call");
      exit(-1);
    }
  //if the server was disconecter print message and end
  if(messageLen == 0){
     printf("\nServer Terminated\n");
     return 0;
  }
  memcpy(&flag, buf, 1);
  switch(flag){
      case 4:
           readBroadCast((char *)buf, messageLen);
           break;
      case 5:
           readMessage((char *)buf, messageLen);
           break;
      case 8:
          return 0;        
          break;
      case 7:
          badHandle(&buf[1]);
          break;
      case 11:
          readNumLen(&buf[1]);
          break;
           
      case 12:
          prossesHandle(buf, handle);
          printf("\t%s\n", handle);     
          break; 
      case 13:
           return -2;
           break;
   

  }


  return messageLen;
}

void readBroadCast(char * buf,int len){

  char handle[MAXHANDLE + 1];
  int offset = 1; //skipping the flags

  offset += prossesHandle((uint8_t * ) &buf[offset], handle) + 1;  

  printf("\n%s: %.*s\n",handle, len - offset, &buf[offset] );
}

void readMessage(char * buf,int len){

  char  senderhandle[MAXHANDLE];
  char handle[MAXHANDLE];
  uint8_t numhandles;
  int offset = 1; //skipping the flags
  int i;

  offset += prossesHandle((uint8_t * ) &buf[offset], senderhandle) + 1;  
  memcpy(&numhandles, &buf[offset++], 1);
   
  for(i = 0; i < numhandles; i++){
     
      offset += prossesHandle((uint8_t * ) &buf[offset], handle) + 1;
  
  }
  printf("\n%s: %.*s\n",senderhandle, len - offset, &buf[offset] );
}

void badHandle(uint8_t * buf){
  char handle[MAXHANDLE];
  
  prossesHandle(buf, handle);

  printf("\nClinet with handle %s dose not exist\n", handle);
    

}

void readNumLen(uint8_t * buf){
    
    uint32_t Num;
    memcpy(&Num, buf, 4);

    Num = ntohl(Num);

    printf("Number of clients: %d\n", Num);
    fflush(stdout);
}
