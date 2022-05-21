# udpCode makefile
# written by Hugh Smith - Feb 2021

CC = gcc
CFLAGS = -g -Wall


SRC = networks.c  gethostbyname.c safeUtil.c UDP.o
OBJS = networks.o gethostbyname.o safeUtil.o UDP.o

#uncomment next two lines if your using sendtoErr() library
LIBS += libcpe464.2.21.a -lstdc++ -ldl
CFLAGS += -D__LIBCPE464_

all:  rcopy server test

rcopy: rcopy.c $(OBJS) 
	$(CC) $(CFLAGS) -o rcopy rcopy.c $(OBJS) $(LIBS)

server: server.c $(OBJS) 
	$(CC) $(CFLAGS) -o server server.c  $(OBJS) $(LIBS)

test: testWindows.c windows.o UDP.o 
	$(CC) -o test $(CFLAGS) testWindows.c windows.o UDP.o $(LIBS)

UDP.o: UDP.c UDP.h checksum.h  
	$(CC) $(CFLAGS) -c UDP.c 

windows.o: windows.c windows.h checksum.h  
	$(CC) $(CFLAGS) -c windows.c

%.o: %.c *.h 
	gcc -c $(CFLAGS) $< -o $@ 

cleano:
	rm -f *.o

clean:
	rm -f server rcopy *.o

