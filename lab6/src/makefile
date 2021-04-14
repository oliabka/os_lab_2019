TARGETS=client server
CC=gcc
CFLAGS=-I.

.PHONY: all clean

client: multmodulo.h multmodulo.o
	$(CC) -pthread -o client multmodulo.o client.c  $(CFLAGS)
	
server: multmodulo.h multmodulo.o
	$(CC) -pthread -o server multmodulo.o server.c  $(CFLAGS)

multmodulo.o : multmodulo.h
	$(CC) -o multmodulo.o -c multmodulo.c $(CFLAGS)

libMULTMODULO.a: multmodulo.o
	ar rc libMULTMODULO.a multmodulo.o
	ranlib libMULTMODULO.a

all: $(TARGETS)

clean :
	rm client server multmodulo.o