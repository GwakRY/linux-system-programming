all: smallsh

smallsh: main.o smallsh.o
	gcc -O -o smallsh main.o smallsh.o

main.o: main.c smallsh.h
	gcc -O -c -o main.o main.c

smallsh.o: smallsh.c smallsh.h
	gcc -O -c -o smallsh.o smallsh.c

clean:
	rm -f *.o smallsh