TARGETS=memory psum
CC=gcc
CFLAGS=-I.

.PHONY: all clean

all: $(TARGETS)

psum: utils.o utils.h 
	$(CC) -o psum utils.o parallel_sum.c  $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

memory:
	$(CC) -o memory process_memory.c $(CFLAGS)

calculate_sum.o:
	$(CC) -c calculate_sum.c -o calculate_sum.o

libCALCULATE_SUM.a: calculate_sum.o
	ar rc libCALCULATE_SUM.a calculate_sum.o
	ranlib libCALCULATE_SUM.a

clean :
	rm memory calculate_sum.o utils.o