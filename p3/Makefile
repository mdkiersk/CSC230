CC = gcc
CFLAGS = -Wall -std=c99 -g

spellcheck: document.o wordlist.o

spellcheck.o: document.h wordlist.h

document.o: document.h

wordlist.o: wordlist.h

clean:
	rm -f document.o wordlist.o spellcheck.o
	rm -f spellcheck
