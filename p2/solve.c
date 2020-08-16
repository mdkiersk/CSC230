/**
 @file solve.c
 @author Matthew Kierski (mdkiersk)
 This serves as the header file in the solve component. This component is responsible for
 verifying whether a given Sudoku grid is solvable, and if so, will solve it.
 */

#include "solve.h"
#include "validate.h"
#include <stdbool.h>

// Value for traversing index array
#define INDEX_INCREMENT 2

extern bool solve(int n, int size, int grid[size][size])
{
    //Check to see how many empty cells exist so we can initialize array
    int numOfEmpty = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == 0) {
                numOfEmpty++;
            }
        }
    }
    
    //Make new array and fill with the empty positions from 2D array
    int emptyCells[numOfEmpty + numOfEmpty];
    int emptySize = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == 0) {
                emptyCells[emptySize] = i;
                emptyCells[emptySize + 1] = j;
                emptySize = emptySize + INDEX_INCREMENT;
            }
        }
    }

    //Attempt to solve grid, returning false if impossible
    int index = 0;
    int lastAdded = 0;
    for (int i = 1; i <= size; i++) {
        if (numOfEmpty == 0) {
            break;
        }
        grid[emptyCells[index]][emptyCells[index + 1]] = i;
        if (validateAllWithSpaces(n, size, grid)) {
            index = index + INDEX_INCREMENT;
            numOfEmpty--;
            lastAdded = i;
            i = 0;
        }
        else if (!validateAllWithSpaces(n, size, grid) && i == size && index != 0) {
            grid[emptyCells[index]][emptyCells[index + 1]] = 0;
            index = index - INDEX_INCREMENT;
            numOfEmpty++;
            i = lastAdded;
            if (lastAdded >= size) {
                return false;
            }
        }
        else if (!validateAllWithSpaces(n, size, grid) && i == size && index == 0) {
            return false;
        }
    }
    return true;
}

extern bool validateAllWithSpaces(int n, int size, int grid[size][size])
{
    return validateRowsWithSpaces(size, grid) && validateColsWithSpaces(size, grid) &&
        validateSquaresWithSpaces(n, size, grid);
}
