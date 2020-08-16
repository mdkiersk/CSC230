CC = gcc
CFLAGS = -Wall -std=c99 -g

all: conceal extract

conceal: bits.o image.o

extract: bits.o image.o

conceal.o: bits.h image.h

extract.o: bits.h image.h

bits.o: bits.h

image.o: image.h

clean:
	rm -f bits.o image.o conceal.o extract.o
	rm -f conceal extract
