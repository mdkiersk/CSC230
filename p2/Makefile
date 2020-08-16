CC = gcc
CFLAGS = -Wall -std=c99 -g

sudoku: solve.o validate.o

sudoku.o: solve.h validate.h

solve.o: solve.h

validate.o: validate.h

clean:
	rm -f solve.o validate.o sudoku.o
	rm -f sudoku
	rm -f output.txt
