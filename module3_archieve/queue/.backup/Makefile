CFLAGS=-Wall -pedantic -std=c11 -I. -g

all: 				test1 test2 test3

queue.o: 		queue.c queue.h
						gcc $(CFLAGS) -c queue.c

test1.o: 		test1.c queue.h
						gcc $(CFLAGS) -c test1.c

test1:			test1.o queue.o
						gcc $(CFLAGS) test1.o queue.o -o test1

test2.o:		test2.c queue.h
						gcc $(CFLAGS) -c test2.c

test2:			test2.o queue.o
						gcc $(CFLAGS) test2.o queue.o -o test2 

test3.o:		test3.c queue.h
						gcc $(CFLAGS) -c test3.c

test3:			test3.o queue.o
						gcc $(CFLAGS) test3.o queue.o -o test3

c:
						rm -f *.o test1 test2 test3
