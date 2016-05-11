CC=gcc
CFLAGS=-Wall -g -O3
INCLUDES=state.h fringe.h

OBJECTS=astar.o fringe.o

all: ${OBJECTS} ${INCLUDES}
	gcc -o astar ${OBJECTS}

clean:
	rm -f *~
	rm -f #*#
	rm -f *.o
