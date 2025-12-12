main.o: main.c
	gcc -c main.c

input_letter.o: input_letter.c
	gcc -c input_letter.c

term.o: term.c
	gcc -c term.c

db.o: db.c
	gcc -c db.c

all: main.o input_letter.o term.o
	gcc -O3 -lsqlite3 main.o input_letter.o term.o db.o

run: all
	./a.out

clean:
	rm a.out *.o
