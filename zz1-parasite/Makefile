all: parasite

parasite: main.o parasite.o
	gcc -o parasite main.o parasite.o

main.o: main.c
	gcc -c main.c 

parasite.o: parasite.c parasite.h
	gcc -c parasite.c

clean:
	rm -f *.o