TARGETS=client server
CC=gcc
CFLAGS=-I.

.PHONY: all

client: 
	$(CC) -pthread -o client client.c  $(CFLAGS)
	
server: 
	$(CC) -pthread -o server server.c  $(CFLAGS)

all: $(TARGETS)

clean :
	rm $(TARGETS)