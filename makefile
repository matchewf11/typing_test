main.o: main.c
	gcc -c main.c
all: main.o
	gcc main.o
run: all
	./a.out
