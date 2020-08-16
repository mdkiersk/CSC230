/**
* Implements the functions declared in model.h Handles mallocing memory for instances of
* structs, as well as initializing structs and returning references.
* @file model.c
* @author Matthew Kierski
*/

#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Initial capacity of attempt list */
#define INITIAL_ATTEMPT_CAP 3
/** Initial capacity of problem list */
#define INITIAL_PROBLEM_CAP 3
/** Initial capacity of contestant list */
#define INITIAL_CONTESTANT_CAP 3

extern Problem *makeProblem( char const *id, char const *name )
{
    Problem *p = (Problem *)malloc(sizeof(Problem));
    strncpy(p->id, id, MAX_ID + 1);
    strncpy(p->name, name, MAX_NAME + 1);
    p->solvedCount = 0;
    p->attemptCount = 0;
    return p;
}

extern void freeProblem( Problem *problem )
{
    free(problem);
}

extern Contestant *makeContestant( char const *id, char const *name )
{
    Contestant *c = (Contestant *)malloc(sizeof(Contestant));
    strncpy( c->id, id, MAX_ID + 1);
    strncpy( c->name, name, MAX_NAME + 1);
    c->aCount = 0;
    c->aCap = INITIAL_ATTEMPT_CAP;
    //Allocate space for array of attempts
    c->aList = (Attempt *)malloc(INITIAL_ATTEMPT_CAP * sizeof(Attempt));
    c->points = 0;
    c->solvedCount = 0;

    return c;
}

extern void freeContestant( Contestant *contestant )
{
    free(contestant->aList);
    free(contestant);
}

extern Contest *makeContest()
{
    Contest *c = (Contest *)malloc(sizeof(Contest));
    //Initialize problem data
    c->pCount = 0;
    c->pCap = INITIAL_PROBLEM_CAP;
    //Allocate Problem list
    c->pList = (Problem **)malloc(INITIAL_PROBLEM_CAP * sizeof(Problem *));
    
    
    //Initialize contestant data
    c->cCount = 0;
    c->cCap = INITIAL_CONTESTANT_CAP;
    //Allocate contestant list
    c->cList = (Contestant **)malloc(INITIAL_CONTESTANT_CAP * sizeof(Contestant *));
    

    return c;
}

extern void freeContest( Contest *contest )
{
    //Free problem list
    for (int i = 0; i < contest->pCount; i++) {
        freeProblem(contest->pList[i]);
    }
    free(contest->pList);

    //Free contestant list
    for (int i = 0; i < contest->cCount; i++) {
        freeContestant(contest->cList[i]);
    }
    free(contest->cList);

    free(contest);
}

extern Problem *findProblem( Contest *contest, char const *id )
{
    for (int i = 0; i < contest->pCount; i++) {
        if (strcmp(contest->pList[i]->id, id) == 0) {
            return contest->pList[i];
        }
    }
    return NULL;
}

extern Contestant *findContestant( Contest *contest, char const *id )
{
    for (int i = 0; i < contest->cCount; i++) {
        if (strcmp(contest->cList[i]->id, id) == 0) {
            return contest->cList[i];
        }
    }
    return NULL;
}
