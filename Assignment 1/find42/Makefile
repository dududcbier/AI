CC=gcc
CFLAGS=-Wall -g -O3
INCLUDES=state.h fringe.h

OBJECTS=search.o fringe.o

all: ${OBJECTS} ${INCLUDES}
	gcc -o search ${OBJECTS}
	gcc ids.c -o ids

clean:
	rm -f *~
	rm -f #*#
	rm -f *.o
