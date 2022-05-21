/* Server side - UDP Code				    */
/* By Hugh Smith	4/1/2017	*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "gethostbyname.h"
#include "networks.h"
#include "safeUtil.h"
#include "UDP.h"
#include "cpe464.h"
#define MAXBUF 1400

void processClient(int socketNum, double error);
int checkArgs(int argc, char *argv[]);

int main ( int argc, char *argv[]  )
{ 
	int socketNum = 0;				
	int portNumber = 0;
        double error = 0;
	portNumber = checkArgs(argc, argv);
        error = atof(argv[1]);
        if(error < 0 || error > 1){
             printf("Error rate must be bettween 0 and 1\n");
             close(-1);
        }

        sendtoErr_init(error, DROP_OFF, FLIP_ON, DEBUG_ON, RSEED_ON);
	socketNum = udpServerSetup(portNumber);

	processClient(socketNum, error);

	close(socketNum);
	
	return 0;
}

void processClient(int socketNum, double error)
{
	int dataLen = 0; 
	char buffer[MAXBUF + 1];	  
	struct sockaddr_in6 client;		
	int clientAddrLen = sizeof(client);	
        uint8_t pduBuffer[MAXBUF + 8];

	buffer[0] = '\0';
	while (buffer[0] != '.')
	{
		dataLen = safeRecvfrom(socketNum, pduBuffer, MAXBUF, 0, (struct sockaddr *) &client, &clientAddrLen);
                        
	
		printf("Received message from client with ");
	        outputPDU(pduBuffer, dataLen);
		printIPInfo(&client);
		safeSendto(socketNum, pduBuffer, dataLen, 0, (struct sockaddr *) & client, clientAddrLen);
                printf("sent to client\n");

	}
}

int checkArgs(int argc, char *argv[])
{
	// Checks args and returns port number
	int portNumber = 0;

	if (argc != 3 && argc != 2)
	{
		fprintf(stderr, "Usage %s [optional port number]\n", argv[0]);
		exit(-1);
	}
	
	if (argc == 3)
	{
		portNumber = atoi(argv[2]);
	}
	
	return portNumber;
}


