main.o: main.c
	gcc -c main.c

input_letter.o: input_letter.c
	gcc -c input_letter.c

term.o: term.c
	gcc -c term.c

db.o: db.c
	gcc -c db.c

typing.o: typing.c
	gcc -c typing.c

cli.o: cli.c

all: main.o input_letter.o term.o db.o typing.o cli.o
	gcc -O3 -lsqlite3 main.o input_letter.o term.o db.o typing.o cli.o

clean:
	rm a.out *.o *.db
