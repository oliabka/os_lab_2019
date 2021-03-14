TARGETS=memory parallel_min_max
CC=gcc
CFLAGS=-I.

.PHONY: all clean

all: $(TARGETS)

psum: utils.o utils.h sum.o sum.h
	$(CC) -o psum utils.o sum.o parallel_sum.c  $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

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

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm memory calculate_sum.o utils.o