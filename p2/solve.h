/**
@file solve.h
@author Matthew Kierski (mdkiersk)
This serves as the header file in the solve component. This component is responsible for
verifying whether a given Sudoku grid is solvable, and if so, will solve it.
*/

#include <stdbool.h>

/**
 Attempts to solve a given, incomplete Sudoku grid.
 @param n dimension of subgrids
 @param size dimension of entire grid
 @param 2D array representing Sudoku gird
 @return true if solved, false otherwise
*/
bool solve(int n, int size, int grid[size][size]);

/**
Helper method to evaluate all validations at once
@param n size of subgrid
@param size size of whole grid
@param grid[size][size] grid to evalutate
@return true if all validation tests pass, false otherwise
*/
bool validateAllWithSpaces(int n, int size, int grid[size][size]);
