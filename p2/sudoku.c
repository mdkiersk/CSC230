/**
@file sudoku.c
@author Matthew Kierski (mdkiersk)
This file serves as the main component. It will be responsible for reading in the puzzle size
and value in the puzzle. It will use validate.c and solve.c to validate a puzzle and solve it when
necessary.
*/

#include "solve.h"
#include "validate.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// Proepr size for 4X4 grid
#define FOURXFOUR 4
//Proper size for 9x9 grid
#define NINEXNINE 9
//Proper size for 16x16 grid
#define SIXTEENXSIXTEEN 16
//Proper dimension of 4x4 subgrid
#define TWO_SUBGRID 2
//Proper dimension of 9x9 subgrid
#define NINE_SUBGRID 3
//Proper dimension of 16x16 subgrid
#define SIXTEEN_SUBGRID 4

/**
 The starting point of our program. Main will read in the input, creating a new array of the
 desired size. It will then check to see if the grid is complete. If so, it will validate the grid,
 printing "Valid" if correct or "Invalid" if not. If the grid is incomplete, it will attempt to
 solve it. If solved, it will print the result to output, and if unable to be solved, it will
 print "Invalid" to output.
 @return exit status
 */
int main()
{
    int size = 0;
    scanf( "%d", &size);
    //Ensure size is valid
    if (size != FOURXFOUR && size != NINEXNINE && size != SIXTEENXSIXTEEN) {
        printf("%s", "Invalid size\n");
        return EXIT_FAILURE;
    }
    int grid[size][size];
    bool completed = true;
    //Determine n
    int n = 0;
    if (size == FOURXFOUR) {
        n = TWO_SUBGRID;
    }
    else if (size == NINEXNINE) {
        n = NINE_SUBGRID;
    }
    else {
        n = SIXTEEN_SUBGRID;
    }
    //Read input and store it into array
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (scanf( "%d", &grid[i][j]) != 1 || grid[i][j] < 0 || grid[i][j] > size) {
                printf( "%s", "Invalid input\n");
                return EXIT_FAILURE;
            }
            if (grid[i][j] == 0) {
                completed = false;
            }
        }
    }
    //Determine if (in)completed, printing proper output
    if (completed) {
        if (validateAllWithSpaces(n, size, grid)) {
            printf( "%s", "Valid\n");
        }
        else {
            printf( "%s", "Invalid\n");
        }
    }
    else {
        if (solve(n, size, grid)) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    printf( "%3d", grid[i][j]);
                    if (j == size - 1) {
                        printf( "\n");
                    }
                }
            }
        }
        else {
            printf( "%s", "Invalid\n");
        }
    }
    return EXIT_SUCCESS;
}
