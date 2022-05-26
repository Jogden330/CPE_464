#ifndef WINDOWS_H
#define WINDOWS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>

typedef struct serverWindow{
	int lower;
        int current;
        int upper;
        int windowsize;
        struct Data ** PDUs;    

} serverWindow;

typedef struct Data{
	int valflag;
        int index;
        uint32_t seqNum;
        int size;
        uint8_t * pdu;
 
} Data;


serverWindow * window_init(int windowsize );


void printServerWindow_metadata(serverWindow * window);


void printEntireWindow(serverWindow * window);


void addPDUtoWindow(serverWindow * window, uint8_t * pduBuff, int size);


void processRR(serverWindow * window, int RR);


int isOpen(serverWindow * window);



#endif
