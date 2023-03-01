CFLAGS=-Wall -pedantic -std=c11 -I.
OFILES=queue.o hash.o webpage.o pageio.o indexio.o threadio.o

all:	$(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

pageio.o: pageio.c pageio.h
				gcc $(CFLAGS) -c pageio.c      

indexio.o: indexio.c indexio.h
				gcc $(CFLAGS) -c indexio.c      

threadio.o: threadio.c threadio.h
				gcc $(CFLAGS) -c threadio.c      

c:
			rm -f 
