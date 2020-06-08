all: parasite

parasite: main.o parasite.o
	gcc -Wall -o $@ main.o parasite.o

main.o: main.c
	gcc -Wall -c $<

parasite.o: parasite.c parasite.h
	gcc -Wall -c $<

clean:
	rm -f *.o parasite