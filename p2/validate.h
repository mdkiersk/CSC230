/**
 @file validate.h
 @author Matthew Kierski (mdkiersk)
 This file serves as a header for the validate component that determines whether a
 given input is a valid Sudoku grid.
*/

#include <stdbool.h>

/**
 Validates whether a row has exactly one digit in the range 1 to size
 @param size size of the row and columns
 @param grid[size][size] 2D array to evaluate
 @return true if valid, false otherwise
*/
bool validateRows(int size, int grid[size][size]);

/**
 Validates whether a column has exaclty one digit in the range 1 to size
 @param size size of the row and columns
 @param grid[size][size] 2D array to evaluate
 @return true if valid, false otherwise
*/
bool validateCols(int size, int grid[size][size]);

/**
 Validates whether every subgrid contains exactly one digit in the range 1 to size
 @param n dimension of subgrid
 @param size size of the row and columns
 @param grid[size][size] 2D array to evaluate
 @return true if valid, false otherwise
*/
bool validateSquares(int n, int size, int grid[size][size]);

/**
 Validates whether a row has no more than one digit in the range 1 to size, with spaces
 @param size size of the row and columns
 @param grid[size][size] 2D array to evaluate
 @return true if valid, false otherwise
*/
bool validateRowsWithSpaces(int size, int grid[size][size]);

/**
 Validates whether a column has no more than one digit in the range 1 to size. with spaces
 @param size size of the row and columns
 @param grid[size][size] 2D array to evaluate
 @return true if valid, false otherwise
*/
bool validateColsWithSpaces(int size, int grid[size][size]);

/**
 Validates whether every subgrid contains exactly one digit in the range 1 to size, with spaces
 @param n dimension of subgrid
 @param size size of the row and columns
 @param grid[size][size] 2D array to evaluate
 @return true if valid, false otherwise
*/
bool validateSquaresWithSpaces(int n, int size, int grid[size][size]);
