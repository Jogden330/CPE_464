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

typedef struct clientWindow{
	int lower;
        int current;
        int upper;
        int windowsize;
        struct Data ** PDUs;    

} clientWindow;

typedef struct Data{
	int valflag;
        int index;
        uint32_t seqNum;
        int size;
        uint8_t * pdu;
 
} Data;


clientWindow * window_init(int windowsize );


void printServerWindow_metadata(clientWindow * window);


void printEntireWindow(clientWindow * window);


void addToWindow(clientWindow * window, uint8_t * pduBuff, int size, uint32_t seqNum);


void processRR(clientWindow * window, int RR);


int findPDU(clientWindow * window, uint8_t * pduBuff, uint32_t seqNum);

int isOpen(clientWindow * window);

int isEmpty(clientWindow * window);

#endif
