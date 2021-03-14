TARGETS=memory psum
CC=gcc
CFLAGS=-I.

.PHONY: all clean

all: $(TARGETS)

psum: utils.o utils.h sum.o sum.h
	$(CC) -o psum utils.o sum.o parallel_sum.c  $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

sum.o : sum.h
	$(CC) -o sum.o -c sum.c $(CFLAGS)

memory:
	$(CC) -o memory process_memory.c $(CFLAGS)

libSUM.a: sum.o
	ar rc libSUM.a sum.o
	ranlib libSUM.a

calculate_sum.o:
	$(CC) -c calculate_sum.c -o calculate_sum.o

clean :
	rm memory calculate_sum.o utils.o