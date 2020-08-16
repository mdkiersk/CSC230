/**
The header file of the pattern component. Declares methods used for matching
and the general Pattern struct from which pattern subclasses will be derived.

@file pattern.h
@author Matthew Kierski
  */

#ifndef PATTERN_H
#define PATTERN_H

#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Superclass for Patterns

/** A short name to use for the Pattern interface. */
typedef struct PatternStruct Pattern;

/**
  Structure used as a superclass/interface for a regular expression
  pattern.  There's a function pointer for an overridable method,
  match(), that reports all the places where this pattern matches a
  given string.  There's also an overridable method for freeing
  resources for the pattern.
*/
struct PatternStruct {
  /** Method for matching this pattern against a given string.  For
      each substring str[ begin ] .. str[ end - 1 ] that matches this
      pattern, this function sets table[ begin ][ end ] to true.

      @param pat pointer to the pattern being matched (essentially, a this
                 pointer).
      @param str input string in which we're finding matches.
      @param len length of str.
      @param table ( len + 1 ) x ( len + 1 ) array of booleans that
                   gets filled in with the substrings where this
                   pattern matches the string.
  */
  void (*match)( Pattern *pat, char const *str, int len, bool (*table)[ len + 1 ] );

  /** Free memory for this pattern, including any subpatterns it contains.
      @param pat pattern to free.
  */
  void (*destroy)( Pattern *pat );
};

/**
  Make a pattern for a single, non-special character, like `a` or `5`.

  @param sym The symbol this pattern is supposed to match.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeLiteralPattern( char sym );

/**
Returns a pattern for a dot pattern
@return a pattern for a dot pattern
*/
Pattern *makeDotPattern();

/**
Returns a pattern for a start anchor
@return a pattern for a start anchor
*/
Pattern *makeStartPattern();

/**
Returns a pattenr for an end anchor
@return a pattern for an end anchor
*/
Pattern *makeEndPattern();

/**
Returns a pattern for a character class
@param chars array of chars to match
@return a pattern for a character class
*/
Pattern *makeCharClassPattern ( char *chars );

/**
Returns a pattern for an alternation pattern
@param p1 a pointer to the first pattern
@param p2 a pointer to the second pattern
@return a pattern for an alternation pattern
*/
Pattern *makeAlternationPattern( Pattern *p1, Pattern *p2);

/**
Returns a pattern for a plus pattern
@param p a pointer to the repeating pattern
@return a pattern for a plus pattern
*/
Pattern *makePlusPattern( Pattern *p );

/**
Returns a pattern for a asterisk pattern
@param p a pointer to the repeating pattern
@return a pattern for a asterisk pattern
*/
Pattern *makeAsteriskPattern( Pattern *p );

/**
Returns a pattern for a queston pattern
@param p a pointer to the repeating pattern
@return a pattern for a question pattern
*/
Pattern *makeQuestionPattern( Pattern *p );

/**
  Make a pattern for the concatenation of patterns p1 and p2.  It
  should match anything that can be broken into two substrings, s1 and
  s2, where the p1 matches the first part (s1) and p2 matches the
  second part (s2).

  @param p1 Subpattern for matching the first part of the string.
  @param p2 Subpattern for matching the second part of the string.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeConcatenationPattern( Pattern *p1, Pattern *p2 );

#endif
