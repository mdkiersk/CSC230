CC = gcc
CFLAGS = -Wall -std=c99 -g

ugrep: pattern.o parse.o

ugrep.o: pattern.h parse.h

pattern.o: pattern.h

parse.o: parse.h

clean:
	rm -f ugrep.o pattern.o parse.o
	rm -f ugrep
