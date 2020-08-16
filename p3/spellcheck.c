/**
This program is the main component. It will call functions in wordlist and document
to load the word lists and spellcheck the document
@file spellcheck.c
@author Matthew Kierski
*/

#include <stdlib.h>
#include "document.h"
#include "wordlist.h"


/**
This is the starting point of our program. It will read in words from the word list and
store them in memory. It will then read in words from a given document and compare them
against the word list. Finally, it will print out the edited version of the document file
@return exit success
*/
int main ( int argc, char *argv[] )
{
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    else if (argc == 2) {
        checkDocument(argv[1]);
    } else {
        for (int i = 1; i < argc - 1; i++) {
            readWords(argv[i]);
        }
        checkDocument(argv[argc - 1]);
    }
    return EXIT_SUCCESS;
}
