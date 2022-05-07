# Makefile for CPE464 tcp test code
# written by Hugh Smith - April 2019

CC= gcc
CFLAGS= -g -Wall
LIBS = 


all:   cclient server

cclient: myClient.c networks.o gethostbyname.o PDUlab.o
	$(CC) $(CFLAGS) -o cclient myClient.c networks.o gethostbyname.o PDUlab.o $(LIBS)

server: myServer.c networks.o gethostbyname.o PDUlab.o pollLib.o safeUtil.o 
	$(CC) $(CFLAGS) -o server myServer.c networks.o gethostbyname.o PDUlab.o pollLib.o safeUtil.o $(LIBS)

PDUlab.o: PDUlab.c PDUlab.h
	$(CC) $(CFLAGS) -c PDUlab.c $(LIBS)

pollLib.o: pollLib.h pollLib.c safeUtil.h
	$(CC) $(CFLAGS) -c pollLib.c   $(LIBS)

safeUtil.o: safeUtil.h safeUtil.c
	$(CC) $(CFLAGS) -c safeUtil.c  $(LIBS)

.c.o:
	gcc -c $(CFLAGS) $< -o $@ $(LIBS)

cleano:
	rm -f *.o

clean:
	rm -f cclient server *.o




