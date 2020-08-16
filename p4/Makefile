CC = gcc
CFLAGS = -Wall -std=c99 -g

contest: model.o command.o

contest.o: model.h command.h

model.o: model.h

command.o: command.h

clean:
	rm -f command.o model.o contest.o
	rm -f contest
