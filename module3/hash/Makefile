CFLAGS=-Wall -pedantic -std=c11 -I. -g

all: 				test1 test2 test3

queue.o: 		hash.c hash.h
						gcc $(CFLAGS) -c hash.c

test1.o: 		test1.c hash.h
						gcc $(CFLAGS) -c test1.c

test1:			test1.o hash.o
						gcc $(CFLAGS) test1.o hash.o -o test1

test2.o: 		test2.c hash.h
						gcc $(CFLAGS) -c test2.c

test2:			test2.o hash.o
						gcc $(CFLAGS) test2.o hash.o -o test2

test3.o: 		test3.c hash.h
						gcc $(CFLAGS) -c test3.c

test3:			test3.o hash.o
						gcc $(CFLAGS) test3.o hash.o -o test3


c:
						rm -f *.o test1 test2 test3
