/**
 @file validate.c
 @author Matthew Kierski (mdkiersk)
 A program that validates whether a given row, column, or subgrid matches Sudoku validity
 */

#include "validate.h"
#include <stdbool.h>

/**
 Helper method that checks to see if a grid, converted to a single dimensional array, is valid
 @param size size of the array
 @param array[] array to evaluate
 @return true if valid, false otherwise
 */
bool checkArray(int size, int array[]);

/**
 Helper method that checks to see if a grid, converted to a single dimensional array, is valid
 with spaces
 @param size size of the array
 @param array[] array to evaluate
 @return true if valid, false otherwise
 */
bool checkArrayWithSpaces(int size, int array[]);

extern bool validateRows(int size, int grid[size][size])
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; i < size; j++) {
            int lookAt = grid[i][j];
            for (int k = j + 1; k < size; k++) {
                if (lookAt == grid[i][k] || grid[i][k] < 1 || grid[i][k] > size) {
                    return false;
                }
            }
        }
    }
    return true;
}

extern bool validateCols(int size, int grid[size][size])
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; i < size; j++) {
            int lookAt = grid[i][j];
            for (int k = j + 1; k < size; k++) {
                if (lookAt == grid[k][j] || grid[i][k] < 1 || grid[i][k] > size) {
                    return false;
                }
            }
        }
    }
    return true;
}

extern bool validateSquares(int n, int size, int grid[size][size])
{
    int convertedArray[size];
    int convertedSize = 0;
    for (int i = 0; i <= size - n; i = i + n) {
        for (int j = 0; j <= size - n; j = j + n) {
            for (int k = i; k < i + n; k++) {
                for (int m = j; m < j + n; m++) {
                    convertedArray[convertedSize] = grid[k][m];
                    convertedSize++;
                }
            }
            if (!checkArray(convertedSize, convertedArray)) {
                return false;
            }
        }
    }
    return true;
}

extern bool validateRowsWithSpaces(int size, int grid[size][size])
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int lookAt = grid[i][j];
            for (int k = j + 1; k < size; k++) {
                if ((lookAt == grid[i][k] && lookAt != 0) || lookAt > size) {
                    return false;
                }
            }
        }
    }
    return true;
}

extern bool validateColsWithSpaces(int size, int grid[size][size])
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int lookAt = grid[i][j];
            for (int k = i + 1; k < size; k++) {
                if ((lookAt == grid[k][j] && lookAt != 0) || lookAt > size) {
                    return false;
                }
            }
        }
    }
    return true;
}

extern bool validateSquaresWithSpaces(int n, int size, int grid[size][size])
{
    int convertedArray[size];
    int convertedSize = 0;
    for (int i = 0; i < size ; i = i + n) {
        for (int j = 0; j < size; j = j + n) {
            for (int k = i; k < i + n; k++) {
                for (int m = j; m < j + n; m++) {
                    convertedArray[convertedSize] = grid[k][m];
                    convertedSize++;
                }
            }
            if (!checkArrayWithSpaces(convertedSize, convertedArray)) {
                return false;
            }
            convertedSize = 0;
        }
    }
    return true;
}

bool checkArray(int size, int array[])
{
    for (int i = 0; i < size; i++) {
        int lookAt = array[i];
        for (int j = i + 1; j < size; j++) {
            if (lookAt == array[j] || lookAt < 1 || lookAt > size) {
                return false;
            }
        }
    }
    return true;
}

bool checkArrayWithSpaces(int size, int array[])
{
    for (int i = 0; i < size - 1; i++) {
        int lookAt = array[i];
        for (int j = i + 1; j < size; j++) {
            if ((lookAt == array[j] && lookAt != 0) || lookAt > size) {
                return false;
            }
        }
    }
    return true;
}
