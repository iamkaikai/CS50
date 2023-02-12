CFLAGS=-Wall -pedantic -std=c11 -I. -g

all: 				test1 test2 test3

hash.o: 		hash.c hash.h
						gcc $(CFLAGS) -c hash.c
queue.o:		queue.c queue.h
						gcc $(CFLAGS) -c queue.c

c:
						rm -f *.o 
