all: familiada

familiada: main.o familiada.o
	gcc -Wall -o $@ main.o familiada.o

main.o: main.c
	gcc -Wall -c $<

familiada.o: familiada.c familiada.h
	gcc -Wall -c $<

clean:
	rm -f *.o familiada
