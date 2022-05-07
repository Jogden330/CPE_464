/******************************************************************************
* tcp_server.c
*
* CPE 464 - Program 1
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
#include "linkedlist.h"
#include "comm.h"

#define MAXBUF 1024
#define DEBUG_FLAG 1

int checkArgs(int argc, char *argv[]);
int processClient(int clientSocket, struct List* list);
void connectClient(int clientSocket,struct List * list, uint8_t * buf);
int recFromClient(int clientSocket, uint8_t * buf );
void recAndSendBroadCast(int socket, uint8_t * buf, struct List * list, uint16_t len );
void recAndSendMess(int socket, uint8_t * buf, struct List * list, uint16_t len );
void sendListNum(int clientSocket, char * buf, struct List * list);
void serverControl(int ServerSocket, struct List* list);
void addNewClient(int socket);

int main(int argc, char *argv[])
{
	int serverSocket = 0;   //socket descriptor for the server socket
	int clientSocket = 0;   //socket descriptor for the client socket
	int portNumber = 0;
        struct List* list;

        list = createList();
   
        portNumber = checkArgs(argc, argv);

	serverSocket = tcpServerSetup(portNumber);

        setupPollSet();  
	addToPollSet(serverSocket);
        while(1){
        	serverControl(serverSocket, list);
        }
	close(clientSocket);
	close(serverSocket);

	
	return 0;
}

void addNewClient(int socket){
    
   int newSocket = tcpAccept(socket, DEBUG_FLAG);
   addToPollSet(newSocket);

}

void serverControl(int ServerSocket, struct List * list){
  
  int Client;
  Client = pollCall(-1);
  struct Node* clientNode; 

  if(Client == ServerSocket){
     addNewClient(Client);
  } else {
      if( processClient(Client, list) == 0){
        clientNode = getClientSocket(list, Client);
        removeList(list, clientNode);
	removeFromPollSet(Client);
	close(Client);
     }      
  } 
   
}

int processClient(int clientSocket, struct List * list){


	uint8_t buf[MAXBUF];
        uint8_t flag;
        uint16_t len;
        struct  Node * client;

        if((len = recFromClient(clientSocket, buf)) == 0){
		return 0;
        }
              
        memcpy(&flag, buf, 1);
        switch(flag){

            case 1: 
                 connectClient(clientSocket, list,  &buf[1]);
                 break;
            case 4:
                 recAndSendBroadCast(clientSocket, buf, list, len);
                 break;
            case 5:
                 recAndSendMess(clientSocket, buf, list, len);
                 break;
            case 8: //client exit
                 sendCommand(clientSocket, (char *) buf,8);
                 break;
            case 10:
                sendListNum(clientSocket,(char *) buf ,list);
                 client = list->first;
                 while(client != NULL){
            
                      sendHandle(clientSocket, (char *) buf, 12, client->handle);
                      client = client->next;

                 }
                 sendCommand(clientSocket, (char *) buf, 13) ;
                                         
                 break;

        }
        return 1;
}

void recAndSendBroadCast(int socket, uint8_t * buf, struct List * list, uint16_t len ){

 int offset = 1; //stat after the flag
 struct Node * client;
 char handle[MAXHANDLE];
 client = list->first;

 offset += prossesHandle(&buf[offset], handle) + 1;

 while(client != NULL){

    if(strcmp(handle, client->handle)){
      sendTo(client->data,buf, len);
       
    }
    client = client->next;
 }
}

void recAndSendMess(int socket, uint8_t * buf, struct List * list, uint16_t len ){

 uint8_t numhandles;
 int offset = 1;
 struct Node * client;
 char errorbuf[MAXBUF];
 char handle[MAXHANDLE];
 int i;


 //prosses the clinents nandle
 offset += prossesHandle(&buf[offset], handle) + 1;

 memcpy(&numhandles, &buf[offset++], 1 );

 for(i = 0; i < numhandles; i++){
   
   offset += prossesHandle(&buf[offset], handle) + 1;

   if((client = getClientHandle(list, handle)) == NULL){
      sendHandle(socket, errorbuf, 7 , handle); 
   } else {
      sendTo(client->data,buf, len);
   }
 } 
      
}

void sendListNum(int clientSocket, char * buf, struct List * list){
   uint8_t flag = 11;
   uint32_t Num;

   Num = htonl(list->NumClient);
   
   memcpy(buf, &flag, 1);
   memcpy(&buf[1], &Num, 4);
   
   sendTo(clientSocket, (uint8_t *) buf, 5);
   

}


void connectClient(int clientSocket,struct List * list, uint8_t * buf){

     char handle[MAXHANDLE];

     prossesHandle(buf,handle);

     if((getClientHandle(list, handle)) == NULL){
         insertList(list, clientSocket, handle);
         sendCommand(clientSocket, (char *) buf, 2);
      } else {
          sendCommand(clientSocket, (char *) buf, 3);
          removeFromPollSet(clientSocket);
          close(clientSocket);
      }
}

int recFromClient(int clientSocket, uint8_t * buf )
{
        
	int messageLen = 0;
	
	//now get the data from the client_socket
       /* struct pseudoheader *header; */
       	if ((messageLen = recvPDU(clientSocket, buf, MAXBUF)) < 0)
	{
		perror("recv call");
		exit(-1);
	}
                    
        return messageLen;        
      
}



int checkArgs(int argc, char *argv[])
{
	// Checks args and returns port number
	int portNumber = 0;

	if (argc > 2)
	{
		fprintf(stderr, "Usage %s [optional port number]\n", argv[0]);
		exit(-1);
	}
	
	if (argc == 2)
	{
		portNumber = atoi(argv[1]);
	}
	
	return portNumber;
}

