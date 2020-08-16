/**
 * This header file provides struct data for Problems, Attempts, Contestants,
 * and Contests in the program. Is also responsible for any behavior associated with
 * creation and searching of data.
 * @file model.h
 * @author Matthew Kierski
 */

#include <stdbool.h>

/** Maximum length of a person or problem unique id. */
#define MAX_ID 16

/** Maximum length of a person or problem name. */
#define MAX_NAME 40

/** Representation for a problem in the contest. */
typedef struct {
  /** Unique ID for this problem, containing no whitespace */
  char id[ MAX_ID + 1 ];

  /** Name of this problem (maybe not unique, and maybe with whitespace). */
  char name[ MAX_NAME + 1 ];

  /** Number of times this problem has been solved */
  int solvedCount;

  /** Number of times this problem has been attempted */
  int attemptCount;

} Problem;

/** Record for an attempt to solve a problem. */
typedef struct {
  /** Problem this attempt was for. */
  Problem *problem;

  /** Was it a successful attempt. */
  bool solved;
} Attempt;

/** Representation for a person or a team in the competition. */
typedef struct {
  /** Unique ID for this person (containing no whitespace). */
  char id[ MAX_ID + 1 ];

  /** Name for this person (maybe not unique and maybe whitespace). */
  char name[ MAX_NAME + 1 ];

  /** Number of attempts. */
  int aCount;

  /** List of all attempts. */
  Attempt *aList;

  /** Capacity of the aList array. */
  int aCap;

  /** Amount of penalty points accrued */
  int points;

  /** Number of problems solved by this contestant */
  int solvedCount;

} Contestant;

/** Representation for the whole contest, containing a resizable list of problems
    and a resizable lit of contestants. */
typedef struct {
  /** Number of problems. */
  int pCount;

  /** List of all problems. */
  Problem **pList;

  /** Capacity of the pList array. */
  int pCap;

  /*/ Number of contestants. */
  int cCount;

  /** List of all the contestants. */
  Contestant **cList;

  /** Capacity of the current cList array. */
  int cCap;
} Contest;

/**
* This dynamically allocates an instance of Problem, initializes its fields and returns a pointer.
* It copies the given name and id fields into the new struct's fields.
* @param *id ID to set
* @param *name name to set
* @return new instance of Problem
*/
Problem *makeProblem( char const *id, char const *name );

/**
* This frees the memory of an associated problem
* @param *problem pointer to problem to free
*/
void freeProblem( Problem *problem );

/**
* This dynamically allocates an instance of Contestant, initializing its fields to
* given fields and returns its pointer.
* @param *id ID to set
* @param *name name to set
* @return new instance of Contestant
*/
Contestant *makeContestant( char const *id, char const *name );

/**
* Frees the memory of given contestant
* @param *contestant pointer of contestant to free
*/
void freeContestant( Contestant *contestant );

/**
* This dynamically allocates an instance of Contest, initializing its fields and
* returning a pointer to it.
* @return new instance of Contest
*/
Contest *makeContest();

/*
* Frees the memory associated with a given contest
* @param *contest pointer to contest to free
*/
void freeContest( Contest *contest );

/**
* Given a contest and a problem ID, this function returns the pointer to the problem
* with given ID, or NULL if it doesn't exist
* @param *contest contest to search by
* @param *id ID problem ID to search by
* @return problem associated with given data, NULL if doesn't exist
*/
Problem *findProblem( Contest *contest, char const *id );

/**
* Given contest and contestant ID, returns a pointer to the corresponding contestant.
* or NULL if doesn't exist
* @param contest contest to search by
* @param *id contestant ID to search by
* @return contestant associated with given data, NULL if doesn't exist
*/
Contestant *findContestant( Contest *contest, char const *id );
