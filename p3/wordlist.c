/**
Part of the wordlist component. Implements the methods needed for reading word lists from files,
storing and managing the in-memory word list, and checking words against the list.
@file wordlist.c
@author Matthew Kierski
*/

#include "wordlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** Return value for lookupWord when word is pronoun */
#define PRONOUN 2
/** Return value for lookupWord when word is non pronoun */
#define NON_PRONOUN 1
/** Return value for lookupWord when word is not found */
#define WORD_NOT_FOUND 0
/** Max length of word (including null terminator) */
#define WORD_LEN 13
/** Max length of word list */
#define MAX_WORDS 40000

//Global for word list
char static wordList[MAX_WORDS][WORD_LEN];

//Global for size of word list
static int count = 0;

extern bool wordChar( char ch ) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '\'');
}

extern int lookupWord( char const word[] )
{
    for (int i = 0; i < count; i++) {
        if (strcmp(word, wordList[i]) == 0) {
            int length = strlen(word);
            for (int j = 0; j < length; i++) {
                if (word[i] >= 'A' && word[i] <= 'Z') {
                    return PRONOUN;
                } else {
                    return NON_PRONOUN;
                }
            }
        }
    }
    return WORD_NOT_FOUND;
}

extern void readWords (char const fileName[] )
{
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        fprintf(stderr, "Can't open file: %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    char word[WORD_LEN];
    while (fscanf(fp, "%12s", word) == 1 ) {
        bool duplicate = false;
        if (count > MAX_WORDS) {
            fprintf(stderr, "Too many words\n");
            exit(EXIT_FAILURE);
        }
        //Ensure valid word
        for (int i = 0; i < strlen(word); i++) {
            if (!wordChar(word[i])) {
                fprintf(stderr, "Invalid word at %s:%d\n", fileName, count);
                exit(EXIT_FAILURE);
            }
        }

        if (count == 0) {
            strcpy(wordList[0], word);
            count++;
        } else {
            for (int i = 0; i < count; i++) {
                if (strcmp(word, wordList[i]) == 0) {
                    duplicate = true;
                }
            }
        }
        if (!duplicate) {
            strcpy(wordList[count], word);
            count++;
        }
    }
    fclose(fp);
}
