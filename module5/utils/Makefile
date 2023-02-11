CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=queue.o hash.o webpage.o pageio.o

all:	$(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

pageio.o: pageio.c pageio.h
				gcc $(CFLAGS) -c pageio.c      

c:
			rm -f 
