/**
* Starting point of our program. Will read input from the user and call necessary
* functions to perform.
* @file contest.c
* @author Matthew Kierski
*/

#include "model.h"
#include "command.h"
#include <stdlib.h>


int main()
{
    Contest *c = makeContest();
    processCommand(c);
    freeContest(c);

    return EXIT_SUCCESS;
}
