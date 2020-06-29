CC=gcc
CFLAGS=-Wall -g
DEPS=provider.h

all: provider

%.o: %.c %(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

provider: main.o provider.o
	$(CC) $(CFLAGS) -o $@ main.o provider.o	-lrt

clean:
	rm -f *.o provider

