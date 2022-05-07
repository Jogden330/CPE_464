#ifndef COMM_H
#define COOM_H

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

#define MAXHANDLE 101

int sendCommand(int socket,char * buff, uint8_t flag);
int  sendTo(int socketNum, uint8_t * sendBuf, int sendlen);
void sendHandle(int socketNum, char * buf, uint8_t flag, char * handle);
int prossesHandle(uint8_t * buf, char * handle);
int addHandleToMess(uint8_t * buf, char * handle);

#endif
