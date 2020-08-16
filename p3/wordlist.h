/**
Serves as the header file of the wordlist component. Provides methods that will
support reading word list files, storing and managing the in-memory word list, and
checking words against the list.
@file wordlist.h
@author Matthew Kierski
*/
#include <stdbool.h>

/**
Determines if a given character can be part of a valid word
@param ch character to evaluate
@return true if valid char, false otherwise
*/
bool wordChar( char ch );

/**
Given a word from document, looks up the word list and returns whether a given
word is a proper noun, non-proper noun that is spelled correctly, or an inavlid word
@param word word to evaluate
@return 2 if proper noun, 1 if nonproper, 0 if invalid
*/
int lookupWord( char const word[] );

/**
Reads all words from a given file and adds new unique words to in-memory word list.
@param fileName name of file to read
*/
void readWords (char const fileName[] );
