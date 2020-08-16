#!/bin/bash
FAIL=0

# Function to run a test and check its output and exit status against expected.
runTest() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.html

  echo "Test $TESTNO: ./sudoku < input-$TESTNO.txt > output.txt"
  ./sudoku < input-$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test $TESTNO FAILED - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure the output matches the expected output.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** Test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  echo "Sudoku test $TESTNO PASS"
  return 0
}

# Build the program.
make clean
make

# Run individual tests.
if [ -x sudoku ] ; then
    runTest 01 0
    runTest 02 0
    runTest 03 0
    runTest 04 0
    runTest 05 0
    runTest 06 0
    runTest 07 0
    runTest 08 0
    runTest 09 0
    runTest 10 0
    runTest 11 0
    runTest 12 0
    runTest 13 0
    runTest 14 1
    runTest 15 1
    runTest 16 1

else
    echo "**** The sudoku program didn't compile successfully"
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
