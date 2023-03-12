CFLAGS=-Wall -pedantic -std=c11

all: 						test1 test2 test3

integrate.o: 		integrate.c integrate.h
								gcc $(CFLAGS) -c integrate.c

test1.o: 				test1.c integrate.h
								gcc $(CFLAGS) -c test1.c

test1:					test1.o integrate.o
								gcc $(CFLAGS) test1.o integrate.o -o test1

test2.o: 				test2.c integrate.h
								gcc $(CFLAGS) -c test2.c

test2:					test2.o integrate.o
								gcc $(CFLAGS) test2.o integrate.o -o test2

test3.o: 				test3.c integrate.h
								gcc $(CFLAGS) -c test3.c

test3:					test3.o integrate.o
								gcc $(CFLAGS) test3.o integrate.o -o test3

c:
								rm -f *.o test1 test2 test3
