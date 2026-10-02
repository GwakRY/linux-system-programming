all: project1

project1: main.o dir_explorer.o
	gcc -O -o project1 main.o dir_explorer.o

main.o: main.c dir_explorer.h
	gcc -O -c -o main.o main.c

dir_explorer.o: dir_explorer.c dir_explorer.h
	gcc -O -c -o dir_explorer.o dir_explorer.c

clean:
	rm -f *.o project1

