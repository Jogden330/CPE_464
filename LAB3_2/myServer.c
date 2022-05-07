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

#define MAXBUF 1024
#define DEBUG_FLAG 1

int prossesClient(int clientSocket);
int checkArgs(int argc, char *argv[]);
int processClient(int clientSocket);

void serverControl(int ServerSocket);
void addNewClient(int socket);

int main(int argc, char *argv[])
{
	int serverSocket = 0;   //socket descriptor for the server socket
	int clientSocket = 0;   //socket descriptor for the client socket
	int portNumber = 0;
    
        portNumber = checkArgs(argc, argv);

	serverSocket = tcpServerSetup(portNumber);

        setupPollSet();  
	addToPollSet(serverSocket);
        while(1){
        	serverControl(serverSocket);
        }
	close(clientSocket);
	close(serverSocket);

	
	return 0;
}

void addNewClient(int socket){
    
        int newSocket = tcpAccept(socket, DEBUG_FLAG);
        addToPollSet(newSocket);

}

void serverControl(int ServerSocket){
  
  int Client;
  Client = pollCall(-1);

  if(Client == ServerSocket){
     addNewClient(Client);
  } else {
      if( processClient(Client) == 0){
        
	removeFromPollSet(Client);
	close(Client);
     }      
  } 
   
}

int processClient(int clientSocket)
{
        
	uint8_t buf[MAXBUF];
	int messageLen = 0;
	
	//now get the data from the client_socket
       /* struct pseudoheader *header; */
       	if ((messageLen = recvPDU(clientSocket, buf, MAXBUF)) < 0)
	{
		perror("recv call");
		exit(-1);
	}	
                
       if((!strcmp("exit",(char *) buf)|| (messageLen == 0))){
                 return 0;

       }
                 
	printf("Message received, length: %d Data: %s\n", messageLen,(char *) buf);
        
        if((sendPDU(clientSocket, buf, messageLen))< 0){
           perror("send call");
           exit(-1);
        }
        
        return 1;        

       
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

