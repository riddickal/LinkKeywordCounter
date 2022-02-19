CC=gcc
CFLAGS=-c -Wall -g 

all: main

main: web.o main.o
	$(CC) web.o main.o -o main -lpthread -lcurl

web.o: web.c
	$(CC) $(CFLAGS) web.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	/bin/rm -f main *.o *.gz

run:
	./main url.txt

tarball:
	tar -cvzf Riddick.tar.gz *

