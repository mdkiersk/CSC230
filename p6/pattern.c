/**
Responsible for holding structs and methods for different patterns foind in parsing.

@file pattern.c
@author Matthew Kierski
  */

#include "pattern.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Max length of input line */
#define MAX_LINE_LEN 100

/**
   A simple function that can be used to free the memory for any
   pattern that doesn't allocate any additional memory other than the
   struct used to represent it (e.g., if it doesn't contain any
   sub-patterns).  It's like a default implementation of the destroy
   method.

   @param pat The pattern to free memory for.
*/
static void destroySimplePattern( Pattern *pat )
{
  // If we don't need fields that are specific to the sub-type, we can just
  // free the block of memory where the object is stored.
  free( pat );
}

/**
   Type of pattern used to represent a single, ordinary symbol,
   like 'a' or '5'.
*/
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  /** Symbol this pattern is supposed to match. */
  char sym;
} LiteralPattern;


// Overridden match() method for a LiteralPattern
static void matchLiteralPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  // Cast down to the struct type pat really points to.
  LiteralPattern *this = (LiteralPattern *) pat;

  // Find all occurreces of the symbol we're supposed to match, and
  // mark them in the match table as matching, 1-character substrings.
  for ( int i = 0; i < len; i++ )
    if ( str[ i ] == this->sym )
      table[ i ][ i + 1 ] = true;
}

// Documented in the header.
Pattern *makeLiteralPattern( char sym )
{
  // Make an instance of LiteralPattern, and fill in its state.
  LiteralPattern *this = (LiteralPattern *) malloc( sizeof( LiteralPattern ) );

  this->match = matchLiteralPattern;
  this->destroy = destroySimplePattern;
  this->sym = sym;

  return (Pattern *) this;
}

// Overridden match() method for a LiteralPattern
static void matchDotPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  // Find all occurreces of the symbol we're supposed to match, and
  // mark them in the match table as matching, 1-character substrings.
  for ( int i = 0; i < len; i++ )
    table[ i ][ i + 1 ] = true;
}

//Documented in header
Pattern *makeDotPattern()
{
  // Make an instance of LiteralPattern, and fill in its state.
  LiteralPattern *this = (LiteralPattern *) malloc( sizeof( LiteralPattern ) );

  this->match = matchDotPattern;
  this->destroy = destroySimplePattern;

  return (Pattern *) this;
}

// Overridden match() method for a LiteralPattern
static void matchStartPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  table[0][0] = true;
}

//Documented in header
Pattern *makeStartPattern()
{
  // Make an instance of LiteralPattern, and fill in its state.
  LiteralPattern *this = (LiteralPattern *) malloc( sizeof( LiteralPattern ) );

  this->match = matchStartPattern;
  this->destroy = destroySimplePattern;

  return (Pattern *) this;
}

// Overridden match() method for a LiteralPattern
static void matchEndPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  table[len - 1][len - 1] = true;
}

//Documented in header
Pattern *makeEndPattern()
{
  // Make an instance of LiteralPattern, and fill in its state.
  LiteralPattern *this = (LiteralPattern *) malloc( sizeof( LiteralPattern ) );

  this->match = matchEndPattern;
  this->destroy = destroySimplePattern;

  return (Pattern *) this;
}

/**
   Representation for a type of pattern that contains two sub-patterns
   (e.g., concatenation).  This representation could be used by more
   than one type of pattern, as long as it uses a pointer to a
   different match() function.
*/
typedef struct {
  // Fields from our superclass.
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  /** Pointers to the two sub-patterns. */
  Pattern *p1, *p2;
} BinaryPattern;

// destroy function used for BinaryPattern
static void destroyBinaryPattern( Pattern *pat )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Free our two sub-patterns.
  this->p1->destroy( this->p1 );
  this->p2->destroy( this->p2 );
  // Free the struct representing this object.
  free( this );
}

// match function for a BinaryPattern used to handle concatenation.
static void matchConcatenationPattern( Pattern *pat, char const *str, int len,
                                       bool (*table)[ len + 1 ] )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Two more tables for matching sub-expressions.
  bool (*tbl1)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  bool (*tbl2)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );

  //  Let our two sub-patterns figure out everywhere they match.
  this->p1->match( this->p1, str, len, tbl1 );
  this->p2->match( this->p2, str, len, tbl2 );

  // Then, based on their matches, look for all places where their
  // concatenaton matches.  Check all substrings of the input string.
  for ( int begin = 0; begin <= len; begin++ )
    for ( int end = begin; end <= len; end++ ) {

      // For the [ begin, end ) range, check all places where it could
      // be split into two substrings, the first matching p1 and the second
      // matching p2.
      for ( int k = begin; k <= end; k++ )
        if ( tbl1[ begin ][ k ] && tbl2[ k ][ end ] )
          table[ begin ][ end ] = true;
    }

  free( tbl1 );
  free( tbl2 );
}

// match function for a BinaryPattern used to handle altenration.
static void matchAlternationPattern( Pattern *pat, char const *str, int len,
                                       bool (*table)[ len + 1 ] )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Two more tables for matching sub-expressions.
  bool (*tbl1)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  bool (*tbl2)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );

  //  Let our two sub-patterns figure out everywhere they match.
  this->p1->match( this->p1, str, len, tbl1 );
  this->p2->match( this->p2, str, len, tbl2 );

  // Then, based on their matches, look for all places where their
  // alternation matches.  Check all substrings of the input string.
  for ( int begin = 0; begin <= len; begin++ ) {
    for ( int end = begin; end <= len; end++ ) {
      if (tbl1[begin][end] || tbl2[begin][end]) {
        table[begin][end] = true;
      }
    }
  }

  free( tbl1 );
  free( tbl2 );
}

//Documented in header
Pattern *makeConcatenationPattern( Pattern *p1, Pattern *p2 )
{
  // Make an instance of Binary pattern and fill in its fields.
  BinaryPattern *this = (BinaryPattern *) malloc( sizeof( BinaryPattern ) );
  this->p1 = p1;
  this->p2 = p2;

  this->match = matchConcatenationPattern;
  this->destroy = destroyBinaryPattern;

  return (Pattern *) this;
}

//Documented in header
Pattern *makeAlternationPattern( Pattern *p1, Pattern *p2 )
{
  // Make an instance of Binary pattern and fill in its fields.
  BinaryPattern *this = (BinaryPattern *) malloc( sizeof( BinaryPattern ) );
  this->p1 = p1;
  this->p2 = p2;

  this->match = matchAlternationPattern;
  this->destroy = destroyBinaryPattern;

  return (Pattern *) this;
}

/**
 * A struct that represents the character class pattern.
 * It holds the functions of all patterns, as well as a char array to hold
 * the characters to match
 */
typedef struct {
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );

  /**Array of chars to match */
  char charClass[MAX_LINE_LEN + 1];

  /** The size of the char array */
  int size;

} CharacterClassPattern;


static void matchCharacterClassPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  CharacterClassPattern *this = (CharacterClassPattern *) pat;
  // Find all occurreces of the symbol we're supposed to match, and
  // mark them in the match table as matching, 1-character substrings.
  for ( int i = 0; i < len; i++ ) {
    for (int k = 0; k < this->size; k++) {
      if ( str[ i ] == this->charClass[k] )
        table[ i ][ i + 1 ] = true;
    }
  }
}

static void destroyCharacterClassPattern( Pattern *pat )
{
  CharacterClassPattern *this = (CharacterClassPattern *) pat;
  free(this);
}

//Documented in header
Pattern *makeCharClassPattern ( char *chars )
{
  CharacterClassPattern *this = (CharacterClassPattern *) malloc( sizeof( CharacterClassPattern ) );

  this->match = matchCharacterClassPattern;
  this->destroy = destroyCharacterClassPattern;
  this->size = 0;

  for (int i = 0; i < strlen(chars); i++) {
    this->charClass[i] = chars[i];
    this->size++;
  }

  return (Pattern *) this;
}

/**
 * A struct that represents repetition patterns such as '+', '*', and '?'.
 * Holds a pointer to the pattern that is repeated.
 */
typedef struct {
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );
  void (*destroy)( Pattern *pat );
  
  /** Pointer to repeating pattern */
  Pattern *p;

} RepetitionPattern;

static void destroyRepetitionPattern( Pattern *pat )
{
  RepetitionPattern *this = (RepetitionPattern *) pat;
  this->p->destroy(this->p);
  free(pat);
}

static void matchPlusPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  RepetitionPattern *this = (RepetitionPattern *) pat;

  bool (*tbl)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  this->p->match( this->p, str, len, tbl );
  
  for (int begin = 0; begin <= len; begin++) {
    for (int end = 0; end <= len; end++) {
      if (tbl[begin][end]) {
        table[begin][end] = true;
        if (tbl[begin + (end - begin)][end + (end - begin)]) {
          for (int i = 0; i < len - end; i++) {
            table[begin][end + i] = true;
          }
        }
      }
    }
  }
  free(tbl);
}

static void matchAsteriskPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  RepetitionPattern *this = (RepetitionPattern *) pat;

  bool (*tbl)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  this->p->match( this->p, str, len, tbl );
  
  for (int begin = 0; begin <= len; begin++) {
    for (int end = 0; end <= len; end++) {
      if (tbl[begin][end]) {
        table[begin][end] = true;
        if (tbl[begin + (end - begin)][end + (end - begin)]) {
          for (int i = 0; i < len - end; i++) {
            table[begin][end + i] = true;
          }
        }
      }
      if (!tbl[begin][end]) {
        table[begin][begin] = true;
      }
    }
  }
  free(tbl);
}

static void matchQuestionPattern( Pattern *pat, char const *str, int len,
                                 bool (*table)[ len + 1 ] )
{
  RepetitionPattern *this = (RepetitionPattern *) pat;

  bool (*tbl)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
  this->p->match( this->p, str, len, tbl );
  
  for (int begin = 0; begin <= len; begin++) {
    for (int end = 0; end <= len; end++) {
      if (!tbl[begin][end]) {
        table[begin][begin] = true;
      }
      else if (tbl[begin][end]) {
        table[begin][end] = true;
      }
    }
  }
  free(tbl);
}

//Documented in header
Pattern *makePlusPattern( Pattern *pat )
{
  RepetitionPattern *this = (RepetitionPattern *) malloc( sizeof( RepetitionPattern ) );
  this->match = matchPlusPattern;
  this->destroy = destroyRepetitionPattern;
  this->p = pat;
  return (Pattern *) this;
}
//Documented in header
Pattern *makeAsteriskPattern( Pattern *pat )
{
  RepetitionPattern *this = (RepetitionPattern *) malloc( sizeof( RepetitionPattern ) );
  this->match = matchAsteriskPattern;
  this->destroy = destroyRepetitionPattern;
  this->p = pat;
  return (Pattern *) this;
}

//Documented in header
Pattern *makeQuestionPattern( Pattern *pat )
{
  RepetitionPattern *this = (RepetitionPattern *) malloc( sizeof( RepetitionPattern ) );
  this->match = matchQuestionPattern;
  this->destroy = destroyRepetitionPattern;
  this->p = pat;
  return (Pattern *) this;
}
