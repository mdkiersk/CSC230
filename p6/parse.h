/**
The header file for the parse component. Declares the starter method in the recursive
structure of parsing methods

@file parse.h
@author Matthew Kierski
  */

#ifndef PARSE_H
#define PARSE_H

#include "pattern.h"

/** Parse the given string into Pattern object.

    @param str string cntaining a pattern.
    @return pointer to a representation of the pattern.
***/
Pattern *parsePattern( char const *str );

#endif
