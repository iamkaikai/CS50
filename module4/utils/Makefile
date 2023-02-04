CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=queue.o hash.o webpage.o

all:	$(OFILES) webpage.o
			ar cr ./libutils.a $(OFILES)

webpage.o: webpage.c webpage.h
				gcc $(CFLAGS) -c webpage.c      

c:
			rm -f 
