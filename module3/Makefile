CFLAGS=-Wall -pedantic -std=c11 -I. -g


all: 				test

list.o: 		list.c list.h
						gcc $(CFLAGS) -c list.c

test.o: 		test.c list.h
						gcc $(CFLAGS) -c test.c

test:				test.o list.o
						gcc $(CFLAGS) test.o list.o -o test
c:
						rm -f *.o test
