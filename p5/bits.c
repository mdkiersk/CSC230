/**
Implements the functions declared in bits.h. This includes a function for returning the boolean
value of a bit at a given position in a byte, as well as a function for setting or unsetting the
bit at a given position in a byte.

@file bits.c
@author Matthew Kierski
*/

#include "bits.h"

extern bool getBit( unsigned char ch, int n )
{
    return (ch >> n) & 1;
}


extern unsigned char putBit( unsigned char ch, int n, bool v )
{
    if (v) {
        return (1 << n) | ch;
    } else {
        return ch &= ~(1 << n);
    }
}
