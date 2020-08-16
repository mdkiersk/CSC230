 /**
* This file implements the functions declared in command.h. It will process each command,
* storing the appropriate data and printing the appropriate values.

* @file command.c
* @author Matthew Kierski
*/


#include "model.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/** String equivalent of valid problem command from user */
#define PROBLEM_COMMAND "problem"
/** String equivalent of valid contestant command from user */
#define CONTESTANT_COMMAND "contestant"
/** String equivalent of valid attempt command from user */
#define ATTEMPT_COMMAND "attempt"
/** String equivalent of valid solved command from user */
#define SOLVED_COMMAND "solved"
/** String equivalent of valid list command from user */
#define LIST_COMMAND "list"
/** String equivalent of valid quit command from user */
#define QUIT_COMMAND "quit"

/** Represents penalty points accrued for unsolved attempts */
#define PENALTY 20

/**
 * Test function to be passed to listProblems. This test function lists all problems
 * @param *problem pointer to problem to evaluate
 * @param *data void pointer to data to evaluate
 * @return true for all problems
 */
bool printAllTest( Problem *problem, void *data )
{
    return true;
}

/**
 * Test function to be passed to listProblems. This test function returns true
 * if a given problem has been solved by a given contestant.
 * @param *problem problem to evaluate
 * @param *data void pointer to point at passed contestant
 * @return return true if this problem has been solved by given contestant, false otherwise.
 */
bool printSolvedForContestant( Problem *problem, void *data )
{
    Contestant *c = data;
    for (int i = 0; i < c->aCount; i++) {
        if (strcmp(problem->id, c->aList[i].problem->id) == 0 && c->aList[i].solved) {
            return true;
        }
    }
    return false;
}

/**
 * Test function to be passed to listProblems. This test function returns false
 * if a given problem has been solved by a given contestant.
 * @param *problem problem to evaluate
 * @param *data void pointer to point at passed contestant
 * @return return false if this problem has been solved by given contestant, true otherwise.
 */
bool printUnsolvedForContestant( Problem *problem, void *data )
{
    Contestant *c = data;
    for (int i = 0; i < c->aCount; i++) {
        if (strcmp(problem->id, c->aList[i].problem->id) == 0 && c->aList[i].solved) {
            return false;
        }
    }
    return true;
}

/**
 * Comparison function to be passed to qsort that defines the equality of problems
 * @param *aptr pointer to pointer to problem in pList
 * @param *bptr pointer to pointer to problem in pList
 * @return -1, 1, or 0 if respectively aptr is less than bptr, vice versa, or equal
 */
static int problemComp( const void *aptr, const void *bptr )
{
    Problem* const *a = aptr;
    Problem* const *b = bptr;

    if ((*a)->solvedCount > (*b)->solvedCount) {
        return -1;
    }
    if ((*a)->solvedCount < (*b)->solvedCount) {
        return 1;
    } else {
        if ((*a)->attemptCount < (*b)->attemptCount) {
            return -1;
        }
        if ((*a)->attemptCount > (*b)->attemptCount) {
            return 1;
        } else {
            return strcmp((*a)->id, (*b)->id);
        }
    }
    return 0;
}

/**
 * Comparison function to be passed to qsort that defines the equality of contestants
 * @param *aptr pointer to pointer to contestant in cList
 * @param *bptr pointer to pointer to contestant in cList
 * @return -1, 1, or 0 if respectively aptr is less than bptr, vice versa, or equal
 */
static int contestantComp( const void *aptr, const void *bptr )
{
    Contestant* const *a = aptr;
    Contestant* const *b = bptr;

    if ((*a)->solvedCount > (*b)->solvedCount) {
        return -1;
    }
    if ((*a)->solvedCount < (*b)->solvedCount) {
        return 1;
    } else {
        if ((*a)->points < (*b)->points) {
            return -1;
        }
        if ((*a)->points > (*b)->points) {
            return 1;
        } else {
            return strcmp((*a)->id, (*b)->id);
        }
    }
    return 0;
}

/**
 * Function responsible for printing a table of a list of problems.
 * @param contest contest to print problems from
 * @param *test test function that accepts a problem and void pointer as parameters to determine
   what problems should be printed.
 * @param *data void data pointer to be used for evaluating problems
 */
static void listProblems( Contest *contest, bool (*test)(Problem *problem, void *data),
    void *data )
{
    qsort(contest->pList, contest->pCount, sizeof(Problem *), problemComp);
    printf("%-16s %-40s %9s %9s\n", "ID", "Name", "Solutions", "Attempts");
    for (int i = 0; i < contest->pCount; i++) {
        if (test(contest->pList[i], data)) {
            printf("%-16s %-40s %9d %9d\n", contest->pList[i]->id, contest->pList[i]->name,
                contest->pList[i]->solvedCount, contest->pList[i]->attemptCount);
        }
    }
}

/**
 * This function is reponsible for printing the table of the list of contestants.
 * @param contest contest to print the contestants of
 */
static void listContestants( Contest *contest )
{
    qsort(contest->cList, contest->cCount, sizeof(Contestant *), contestantComp);
    printf("%-16s %-40s %9s %9s\n", "ID", "Name", "Solved", "Penalty");
    for (int i = 0; i < contest->cCount; i++) {
        printf("%-16s %-40s %9d %9d\n", contest->cList[i]->id, contest->cList[i]->name,
            contest->cList[i]->solvedCount, contest->cList[i]->points);
    }
}

extern bool processCommand( Contest *contest )
{
    static int numOfCommands = 1;
    int pos;
    char command[MAX_ID] = {};
    bool noQuit = true;
    while (noQuit) {
        printf("%d%c ", numOfCommands++, '>');
        if (scanf("%s", command) == EOF) {
            return false;
        }
        printf("\n");
        if (strcmp(command, PROBLEM_COMMAND) == 0) {
            char id[MAX_ID + 1] = {};
            char name[MAX_NAME + 1] = {};
            scanf("%17s%n", id, &pos);
            //Remove space after id
            char next = getchar();
            while (isspace(next)) {
                next = getchar();
            }
            ungetc(next, stdin);
            //Read name
            scanf("%[^\n]%*c", name);
            if (pos - 1 > MAX_ID) {
                printf("%s", "Invalid command\n");
                continue;
            }
            else if (strlen(name) > MAX_NAME) {
                printf("%s", "Invalid command\n");
                continue;
            //Valid command -- process it
            } else {
                //Check if ID exists already
                bool duplicate = false;
                for (int i = 0; i < contest->pCount; i++) {
                    if (strcmp(id, contest->pList[i]->id) == 0) {
                        printf("%s", "Invalid command\n");
                        duplicate = true;
                    }
                }
                //Add problem if not duplicate, resizing when necessary
                if (!duplicate) {
                    if (contest->pCount >= contest->pCap) {
                        contest->pCap *= 2;
                        contest->pList = (Problem **)realloc(contest->pList, contest->pCap *
                            sizeof(Problem *));
                    }
                    contest->pList[contest->pCount++] = makeProblem(id, name);
                }
            }
        }
        else if (strcmp(command, CONTESTANT_COMMAND) == 0) {
            char id[MAX_ID + 1] = {};
            char name[MAX_NAME + 1] = {};
            scanf("%17s%n", id, &pos);
            getchar();
            scanf("%[^\n]%*c", name);
            if (pos - 1 > MAX_ID) {
                printf("%s", "Invalid command\n");
                continue;
            }
            else if (strlen(name) > MAX_NAME) {
                printf("%s", "Invalid command\n");
                continue;
            } else {
                 //Valid command -- process it
                 //Check if ID exists already
                 bool duplicate = false;
                for (int i = 0; i < contest->cCount; i++) {
                    if (strcmp(id, contest->cList[i]->id) == 0) {
                        printf("%s", "Invalid command\n");
                        duplicate = true;
                    }
                }
                if (!duplicate) {
                    if (contest->cCount >= contest->cCap) {
                        contest->cCap *= 2;
                        contest->cList = (Contestant **)realloc
                            (contest->cList, contest->cCap * sizeof(Contestant *));
                    }
                    contest->cList[contest->cCount++] = makeContestant(id, name);
                }
            }
        }
        else if (strcmp(command, ATTEMPT_COMMAND) == 0) {
            char contID[MAX_ID + 1] = {};
            char problemID[MAX_ID + 1] = {};
            scanf("%17s%n", contID, &pos);
            scanf("%17s", problemID);
            if (pos - 1 > MAX_ID) {
                printf("%s", "Invalid command\n");
                continue;
            }
            else if (strlen(problemID) > MAX_ID) {
                printf("%s", "Invalid command\n");
                continue;
            } else {
                //Valid command -- process it
                //Check to see if ID for both exist
                bool pExists = false;
                bool cExists = false;
                Problem *p;
                Contestant *c;
                for (int i = 0; i < contest->pCount; i++) {
                    if (strcmp(problemID, contest->pList[i]->id) == 0) {
                        pExists = true;
                        //Hold problem variable to make an attempt for
                        p = contest->pList[i];
                    }
                }
                for (int i = 0; i < contest->cCount; i++) {
                    if (strcmp(contID, contest->cList[i]->id) == 0) {
                        cExists = true;
                        //Hold contestant variable to make an attempt for
                        c = contest->cList[i];
                    }
                }
                if (pExists && cExists) {
                    //Check to see if already solved
                    bool solved = false;
                    for (int i = 0; i < c->aCount; i++) {
                        if (strcmp(problemID, c->aList[i].problem->id) == 0 &&
                            c->aList[i].solved) {
                            solved = true;
                        }
                    }
                    if (!solved) {
                        //Create attempt
                        Attempt *a = (Attempt *)malloc(sizeof(Attempt));
                        a->solved = false;
                        a->problem = p;
                        //Resize attempt list if needed
                        if (c->aCount >= c->aCap) {
                            c->aCap *= 2;
                            c->aList = (Attempt *)realloc(c->aList, c->aCap * sizeof(Attempt));
                        }
                        c->aList[c->aCount++] = *a;
                        a->problem->attemptCount++;
                        free(a);
                    }
                } else {
                    printf("%s", "Invalid command\n");
                    continue;
                }
            }
        }
        else if (strcmp(command, SOLVED_COMMAND) == 0) {
            char contID[MAX_ID + 1] = {};
            char problemID[MAX_ID + 1] = {};
            scanf("%17s%n", contID, &pos);
            scanf("%17s", problemID);
            if (pos - 1 > MAX_ID) {
                printf("%s", "Invalid command\n");
                continue;
            }
            else if (strlen(problemID) > MAX_ID) {
                printf("%s", "Invalid command\n");
                continue;
            } else {
                //Valid command -- process it
                //Check to see if ID for both exist
                bool pExists = false;
                bool cExists = false;
                Problem *p;
                Contestant *c;
                for (int i = 0; i < contest->pCount; i++) {
                    if (strcmp(problemID, contest->pList[i]->id) == 0) {
                        pExists = true;
                        //Hold problem variable to make an attempt for
                        p = contest->pList[i];
                    }
                }
                for (int i = 0; i < contest->cCount; i++) {
                    if (strcmp(contID, contest->cList[i]->id) == 0) {
                        cExists = true;
                        //Hold contestant variable to make an attempt for
                        c = contest->cList[i];
                    }
                }
                if (pExists && cExists) {
                    //Check to see if already solved
                    bool solved = false;
                    for (int i = 0; i < c->aCount; i++) {
                        if (strcmp(problemID, c->aList[i].problem->id) == 0 &&
                            c->aList[i].solved) {
                            solved = true;
                        }
                    }
                    if (!solved) {
                        //Create attempt
                        Attempt *a = (Attempt *)malloc(sizeof(Attempt));
                        a->solved = true;
                        a->problem = p;
                        //Resize attempt list if needed
                        if (c->aCount >= c->aCap) {
                            c->aCap *= 2;
                            c->aList = (Attempt *)realloc(c->aList, c->aCap * sizeof(Attempt));
                        }
                        c->aList[c->aCount++] = *a;
                        a->problem->attemptCount++;
                        a->problem->solvedCount++;
                        c->solvedCount++;
                        free(a);
                        //Increment appropriate points from contestant's total penalty
                        for (int i = 0; i < c->aCount; i++) {
                            if (strcmp(c->aList[i].problem->id, p->id) == 0 &&
                                !(c->aList[i].solved)) {
                                c->points += PENALTY;
                            }
                        }
                    }
                } else {
                    printf("%s", "Invalid command\n");
                    continue;
                }
            }
        }
        else if (strcmp(command, LIST_COMMAND) == 0) {
            char listType[MAX_ID];
            scanf("%s", listType);
            if (strcmp(listType, "problems") == 0) {
                listProblems(contest, printAllTest, NULL);
            }
            else if (strcmp(listType, "contestants") == 0) {
                listContestants(contest);
            }
            else if (strcmp(listType, "solved") == 0) {
                char contID[MAX_ID + 1];
                scanf("%s", contID);
                bool cExists = false;
                Contestant *c;
                for (int i = 0; i < contest->cCount; i++) {
                    if (strcmp(contID, contest->cList[i]->id) == 0) {
                        cExists = true;
                        c = contest->cList[i];
                    }
                }
                if (cExists) {
                    listProblems(contest, printSolvedForContestant, c);
                } else {
                    printf("%s\n", "Invalid command");
                }
            }
            else if (strcmp(listType, "unsolved") == 0) {
                char contID[MAX_ID + 1];
                scanf("%s", contID);
                bool cExists = false;
                Contestant *c;
                for (int i = 0; i < contest->cCount; i++) {
                    if (strcmp(contID, contest->cList[i]->id) == 0) {
                        cExists = true;
                        c = contest->cList[i];
                    }
                }
                if (cExists) {
                    listProblems(contest, printUnsolvedForContestant, c);
                } else {
                    printf("%s\n", "Invalid command");
                }
            }
            else {
                printf("%s\n", "Invalid command");
            }
        }
        else if (strcmp(command, QUIT_COMMAND) == 0) {
            noQuit = false;
            return false;
        } else {
            //Muck the rest of the line
            printf("%s\n", "Invalid command");
        }
    }
    return true;
}
