TARGETS=tcpclient tcpserver udpclient udpserver 
CC=gcc
CFLAGS=-I.

.PHONY: all clean

tcpclient: 
	$(CC) -pthread -o tcpclient tcpclient.c  $(CFLAGS)
	
tcpserver: 
	$(CC) -pthread -o tcpserver tcpserver.c  $(CFLAGS)

udpclient: 
	$(CC) -pthread -o udpclient udpclient.c  $(CFLAGS)
	
udpserver: 
	$(CC) -pthread -o udpserver udpserver.c  $(CFLAGS)

all: $(TARGETS)

clean :
	rm tcpclient tcpserver udpclient udpserver 