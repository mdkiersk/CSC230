/**
Serves as the implementation of the document header file. Provides methods
responsible for readingthe document, using wordlist to check the spelling of
individual words, and printing outthe document in proper format
@file document.c
@author Matthew Kierski
*/

#include "document.h"
#include "wordlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/** Escape sequence for red character */
#define RED "\033[31m"
/** Escape sequence for blue character */
#define BLUE "\033[34m"
/** Escape sequence for default character */
#define RESET "\033[0m"
/** Return value for lookupWord when word is pronoun */
#define PRONOUN 2
/** Return value for lookupWord when word is non pronoun */
#define NON_PRONOUN 1
/** Return value for lookupWord when word is not found */
#define WORD_NOT_FOUND 0
/** Max length of word (including null terminator) */
#define WORD_LEN 13
/** Max length of line (including null terminator) */
#define LINE_LEN 76

static void checkLine( char const line[], bool ellipsis, bool chopped )
{
    char ch = line[0];
    char word[WORD_LEN];
    word[0] = ch;
    int count = 1;
    int i = 1;
    while (ch != '\0') {
        ch = line[count++];
        if (ch != ' ' && ch != '\n' && ispunct(ch) == 0) {
            word[i++] = ch;
        }
        if (ch == '\0' || ch == '\n' || ch == ' ' || ispunct(ch) != 0) {
            if (lookupWord(word) == WORD_NOT_FOUND) {
                //Print in red
                printf(RED "%s" RESET, word);
            }
            else if (lookupWord(word) == NON_PRONOUN) {
                //Print in default
                printf("%s", word);
            }
            else if (lookupWord(word) == PRONOUN) {
                //Print in blue
                printf(BLUE "%s" RESET, word);
            }
            memset(word, 0, WORD_LEN);
            i = 0;
            if (ch != '\0') {
                printf("%c", ch);
            } else {
                printf("%c", '\n');
            }
        }
    }
}

extern void checkDocument( char const fileName[] )
{
    FILE *fp = fopen(fileName, "r");
    if (!fp) {
        fprintf(stderr, "Can't open file: %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    
    int count = 0;
    char line[LINE_LEN];
    char ch = fgetc(fp);
    while (ch != EOF) {
        line[count] = ch;
        count++;
        ch = fgetc(fp);
        if (ch == '\n') {
            line[count] = '\0';
            checkLine(line, false, false);
            memset(line, 0, LINE_LEN);
            count = 0;
        }
        if (count == LINE_LEN - 1) {
            line[count] = '\0';
            checkLine(line, true, true);
            memset(line, 0, LINE_LEN);
            count = 0;
        }
    }
}
