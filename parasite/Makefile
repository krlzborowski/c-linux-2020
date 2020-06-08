all: parasite

parasite: main.o parasite.o
	gcc -o $@ main.o parasite.o

main.o: main.c
	gcc -c $<

parasite.o: parasite.c parasite.h
	gcc -c $<

clean:
	rm -f *.o parasite