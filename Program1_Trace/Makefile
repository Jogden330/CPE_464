
CC = gcc

CFLAGS = -Wall -g -pedantic 

LDFLAGS = -Wall -g -pedantic -Wextra 

all:  trace

trace: trace.o main.o
	$(CC) $(CFLAGS) -o trace main.o trace.o  checksum.c  -lpcap 


main.o: main.c trace.h
	$(CC) $(CFLAGS) -c main.c

trace.o: trace.c trace.h checksum.h
	$(CC) $(CFLAGS) -c trace.c

clean:
	rm -f trace
