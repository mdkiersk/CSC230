/**
The starting point of our pgroam. Handles input from the user and calls appropriate parse and
pattern methods to construct a pattern and output a highlighted result if there are
matches

@file ugrep.c
@author Matthew Kierski
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pattern.h"
#include "parse.h"

// On the command line, which argument is the pattern.
#define PAT_ARG 1
// On the command line, which argument is the input file.
#define FILE_ARG 2

/** Max length of line allowed */
#define MAX_LINE_LEN 100

/** Color code for reset */
#define RESET "\033[0m"

/** Color code for red */
#define RED "\033[31m"

/** Number of arguments to read from stdin */
#define STDIN 2

/** Number of arguments to read from file input */
#define FILE_INPUT 3

/**
 * A helper method to report the matches in a string given the match table. Highlights
 * matches in red, and skips the line if there are no matches
 *
 * @param const *str input string to evaluate matches in
 * @param len length of string
 * @param *table a 2D array representing matches found in string
 */
static void reportMatches( char const *str, int len,
                    bool (*table)[ len + 1 ] )
{

    //Check to see if there are any matches in table
    bool match = false;
    for (int begin = 0; begin < len; begin++) {
        for (int end = 0; end < len; end++) {
            if (table[begin][end]) {
                match = true;
            }
        }
    }
    //If so, print out accordingly
    if (match) {
        int nonRed = 0;
        for (int begin = 0; begin < len; begin++) {
           bool found = false;
           for (int end = 0; end < len; end++) {
               if (table[begin][end]) {
                   found = true;
                   nonRed = end;
                   printf(RED);
                   for (int k = begin; k < end; k++) {
                       printf("%c", str[k]);
                   }
                   printf(RESET);
               }
           }
           if (!found && nonRed <= begin) {
             printf("%c", str[nonRed++]);
           }
        }
    }
}

/**
 * Starting point of the program. Takes input from the user to determine
 * where to read from. Once a stream is opened, it will read the pattern and
 * input string and call the appropriate parse and pattern methods to
 * construct a boolean table of matches on the stirng. Delegates to reportMatches
 * to print output.
 *
 * @param argc number of arguments in command line
 * @param *argv[] char array of command line arguments
 */
int main(int argc, char *argv[])
{
    if (argc < STDIN || argc > FILE_INPUT) {
        fprintf(stderr, "%s", "usage: ugrep <pattern> [input-file.txt]\n");
        exit(EXIT_FAILURE);
    }
    else if (argc == STDIN) {
        //Read from terminal while we do not come across a line that is too long
        //Line of max length
        char line[MAX_LINE_LEN + MAX_LINE_LEN];
        char *stringPattern = argv[PAT_ARG];
        while (fgets(line, MAX_LINE_LEN + MAX_LINE_LEN, stdin) != NULL) {
            //If line is too long, print error message and quit
            if (strlen(line) > MAX_LINE_LEN) {
              fprintf(stderr, "%s", "Input line too long");
              exit(EXIT_FAILURE);
            }
            Pattern *pattern = parsePattern(stringPattern);
            int len = strlen(line);
            bool (*table)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
            pattern->match(pattern, line, len, table);
            reportMatches(line, len, table);
            pattern->destroy(pattern);
            free(table);
        }
    }
    else if (argc == FILE_INPUT) {
        //Read from file
        FILE *fp = fopen(argv[FILE_ARG], "r");
        //Throw error and quit if file is not found
        if (!fp) {
            fprintf(stderr, "Can't open input file: %s\n", argv[FILE_ARG]);
            exit(EXIT_FAILURE);
        }
        //Read from terminal while we do not come across a line that is too long
        //Line of max length
        char line[MAX_LINE_LEN + MAX_LINE_LEN];
        char *stringPattern = argv[PAT_ARG];
        while (fgets(line, MAX_LINE_LEN  + MAX_LINE_LEN, fp)) {
            //If line is too long, print error message and quit
            if (strlen(line) > MAX_LINE_LEN) {
                fprintf(stderr, "%s", "Input line too long\n");
                exit(EXIT_FAILURE);
            }
            Pattern *pattern = parsePattern(stringPattern);
            int len = strlen(line);
            bool (*table)[ len + 1 ]  = calloc( ( len + 1 ) * ( len + 1 ), sizeof( bool ) );
            pattern->match( pattern, line, len, table );
            reportMatches(line, len, table);
            pattern->destroy(pattern);
            free(table);
        }
        fclose(fp);
    }

    exit(EXIT_SUCCESS);
}
