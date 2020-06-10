all: provider

provider: main.o provider.o
	gcc -Wall -o $@ main.o provider.o -lrt

main.o: main.c
	gcc -Wall -c $<

provider.o: provider.c provider.h
	gcc -c -lrt -Wall $<

clean:
	rm -f *.o provider
