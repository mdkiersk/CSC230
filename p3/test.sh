#!/bin/bash
FAIL=0

# Function to check the output of the program after it's run, including checking
# an output file, standard output, standard error and exit status.
checkResults() {
  TESTNO=$1
  ESTATUS=$2
  STATUS=$3

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test $TESTNO FAILED - Expected an exit status of $ESTATUS, but got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure output matches expected output.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** Test $TESTNO FAILED - output to stdout didn't match expected."
      FAIL=1
      return 1
  fi

  # Make sure the output to stderr matches expected.
  if ! diff -q estderr-$TESTNO.txt stderr.txt >/dev/null 2>&1 ; then
      echo "**** Test $TESTNO FAILED - output to stderr didn't match expected"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Try to get a fresh compile of the project.
make clean
make
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your program."
    FAIL=1
fi

# Run individual tests.  Since they each require different arguments,
# it didn't make as much sense to write a single test function for
# this.
if [ -x spellcheck ] ; then
    rm -f output.txt stderr.txt
    echo "Test 01: ./spellcheck input-01.txt"
    ./spellcheck input-01.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 01 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 02: ./spellcheck words-a.txt input-02.txt"
    ./spellcheck words-a.txt input-02.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 02 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 03: ./spellcheck words-b.txt input-03.txt"
    ./spellcheck words-b.txt input-03.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 03 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 04: ./spellcheck words-c.txt input-04.txt"
    ./spellcheck words-c.txt input-04.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 04 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 05: ./spellcheck words-c.txt input-05.txt"
    ./spellcheck words-c.txt input-05.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 05 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 06: ./spellcheck words-d.txt input-06.txt"
    ./spellcheck words-d.txt input-06.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 06 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 07: ./spellcheck words-e.txt input-07.txt"
    ./spellcheck words-e.txt input-07.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 07 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 08: ./spellcheck words-e.txt words-f.txt input-08.txt"
    ./spellcheck words-e.txt words-f.txt input-08.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 08 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 09: ./spellcheck words-e.txt words-f.txt input-09.txt"
    ./spellcheck words-e.txt words-f.txt input-09.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 09 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 10: ./spellcheck words-e.txt input-10.txt"
    ./spellcheck words-e.txt input-10.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 10 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 11: ./spellcheck words-e.txt input-11.txt"
    ./spellcheck words-e.txt input-11.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 11 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 12: ./spellcheck words-g.txt input-12.txt"
    ./spellcheck words-g.txt input-12.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 12 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 13: ./spellcheck words-h.txt input-13.txt"
    ./spellcheck words-h.txt input-13.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 13 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 14: ./spellcheck words-h.txt words-i.txt input-14.txt"
    ./spellcheck words-h.txt words-i.txt input-14.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 14 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 15: ./spellcheck words-e.txt words-j.txt input-15.txt"
    ./spellcheck words-e.txt words-j.txt input-15.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 15 0 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 16: ./spellcheck words-h.txt words-d.txt input-16.txt"
    ./spellcheck words-h.txt words-d.txt input-16.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 16 1 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 17: ./spellcheck words-k.txt input-17.txt"
    ./spellcheck words-k.txt input-17.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 17 1 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 18: ./spellcheck words-a.txt words-b.txt words-l.txt input-18.txt"
    ./spellcheck words-a.txt words-b.txt words-l.txt input-18.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 18 1 $STATUS

    rm -f output.txt stderr.txt
    echo "Test 19: ./spellcheck words-a.txt words-b.txt words-c.txt input-19.txt"
    ./spellcheck words-a.txt words-b.txt words-c.txt input-19.txt > output.txt 2> stderr.txt
    STATUS=$?
    checkResults 19 1 $STATUS

else
    echo "**** Your program couldn't be tested since it didn't compile successfully."
    FAIL=1

fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
