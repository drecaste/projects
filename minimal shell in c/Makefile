CC = gcc
CFLAGS = -Wall -std=c99 -ggdb

# use "make" to compile
default: msh.c parser.c parser.h
	${CC} ${CFLAGS} -o msh msh.c parser.c parser.h

# use "make run" to run the program
run:
	./msh

# use "make clean" to remove the main file that is created with above compilation^
clean:
	rm -f msh
	