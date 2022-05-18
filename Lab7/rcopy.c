// Client side - UDP Code				    
// By Hugh Smith	4/1/2017		

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

#include "gethostbyname.h"
#include "networks.h"
#include "safeUtil.h"
#include "UDP.h"
#include "cpe464.h"

#define MAXBUF 1400 

void talkToServer(int socketNum, double error, struct sockaddr_in6 * server);
int readFromStdin(char * buffer);
int checkArgs(int argc, char * argv[]);


int main (int argc, char *argv[])
 {
	int socketNum = 0;				
	struct sockaddr_in6 server;		// Supports 4 and 6 but requires IPv6 struct
	int portNumber = 0;
        double error = 0;
	
	portNumber = checkArgs(argc, argv);
	error = atof(argv[1]);
        if(error < 0 || error > 1){
             printf("invaled error must be 0 to 1\n");
             close(-1);
        }
        sendtoErr_init(error, DROP_ON, FLIP_OFF, DEBUG_ON, RSEED_ON);
	socketNum = setupUdpClientToServer(&server, argv[2], portNumber);

	talkToServer(socketNum, error, &server);
	
	close(socketNum);

	return 0;
}

void talkToServer(int socketNum, double error,struct sockaddr_in6 * server)
{
	int serverAddrLen = sizeof(struct sockaddr_in6);
	char * ipString = NULL;
	int dataLen = 0; 
	char buffer[MAXBUF+1];
        uint8_t pduBuffer[MAXBUF + 8];
        int PDUlen;
        uint32_t seqNum = 0;
        uint8_t flag = 3; 
	
	buffer[0] = '\0';
         
	while (buffer[0] != '.')
	{
		dataLen = readFromStdin(buffer);
                PDUlen =  createPDU(pduBuffer, seqNum++, flag,(uint8_t *) buffer, dataLen);
               

	        outputPDU(pduBuffer, PDUlen);
	
		

               safeSendto(socketNum, pduBuffer, PDUlen,  flag,  (struct sockaddr *) server, serverAddrLen);
   
		PDUlen = safeRecvfrom(socketNum, pduBuffer, MAXBUF+7, 0, (struct sockaddr *) server, &serverAddrLen);
                printf("receved from server /n");		
	        outputPDU(pduBuffer, PDUlen);
		// print out bytes received:
		ipString = ipAddressToString(server);
		printf("Server with ip: %s and port %d said it received %s\n", ipString, ntohs(server->sin6_port), buffer);
	      
	}
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

int checkArgs(int argc, char * argv[])
{

        int portNumber = 0;
	
        /* check command line arguments  */
	if (argc != 4)
	{
		printf("usage: %s error-persentage hoso-name port-number \n", argv[0]);
		exit(1);
	}
	
	portNumber = atoi(argv[3]);
		
	return portNumber;
}





