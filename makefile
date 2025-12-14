main.o: main.c
	gcc -c main.c

all: main.o
	gcc -O3 main.o

clean:
	rm *.o
