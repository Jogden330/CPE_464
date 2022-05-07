#ifndef PDULAB_H
#define PDULAB_H

#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

int recvPDU(int clientSocket, uint8_t * dataBuffer, int bufferLen);
int sendPDU(int socketNumber, uint8_t * dataBuffer, int lengthOfData);

#endif
