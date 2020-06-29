CC=gcc
CFLAGS=-Wall -g
DEPS=familiada.h

all: familiada

%.o: %.c %(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

familiada: main.o familiada.o
	$(CC) -Wall -o $@ main.o familiada.o	

clean:
	rm -f *.o familiada
