CC=gcc
CFLAGS=-Wall -g
DEPS=parasite.h

all: parasite

%.o: %.c %(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

parasite: main.o parasite.o
	$(CC) $(CFLAGS) -o $@ main.o parasite.o	-lrt

clean:
	rm -f *.o parasite


