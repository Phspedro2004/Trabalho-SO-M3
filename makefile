CC = gcc
CFLAGS = -Wall -g

all: main

main: main.o filesystem.o
	$(CC) $(CFLAGS) -o main main.o filesystem.o

main.o: main.c filesystem.h
	$(CC) $(CFLAGS) -c main.c

filesystem.o: filesystem.c filesystem.h
	$(CC) $(CFLAGS) -c filesystem.c

clean:
	rm -f *.o main
