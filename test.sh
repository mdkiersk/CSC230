#!/bin/bash
FAIL=0

# Make a fresh copy of the target program
make clean
make
if [ $? -ne 0 ] || [ ! -x ugrep ]; then
  echo "**** Make (compilation) FAILED"
  exit 13
fi

# Check the results of program execution.
# Splitting this out from the test function makes it easier
# to handle some error tests.
checkResults() {
  TESTNO=$1
  ESTATUS=$2

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test failed - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure the output matches the expected output.
  if [ -f expected-$TESTNO.txt ]; then
      diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
      if [ $? -ne 0 ]; then
	  echo "**** Test FAILED - program output doesn't match expected"
	  FAIL=1
	  return 1
      fi
  elif [ -s output.txt ]; then
      # if there' no expected output for this test, the program shouldn't
      # have printed anything.
      echo "**** Test FAILED - shouldn't have printed output to stdout"
      FAIL=1
      return 1
  fi
      
  # Check different things, depending on whether it's successful.
  if [ $ESTATUS -eq 0 ]; then
      if [ -s stderr.txt ]; then
	  echo "**** Test FAILED - shouldn't have printed any error output"
	  FAIL=1
	  return 1
      fi
  else
      # Make sure the output matches the expected output.
      diff -q stderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
      if [ $? -ne 0 ]; then
	  echo "**** Test FAILED - didn't print the right error message"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Test the pattern matching program.
runTest() {
  rm -f output.txt stderr.txt

  echo "Test $1: ./ugrep '$2' input-$1.txt > output.txt 2> stderr.txt"
  ./ugrep "$2" input-$1.txt > output.txt 2> stderr.txt
  STATUS=$?

  checkResults "$1" "$3"
}

# Run each of the test cases
runTest 01 'w' 0
runTest 02 'h' 0
runTest 03 'c' 0
runTest 04 'abc' 0
runTest 05 'a.c' 0

echo "Test 06: ./ugrep 'a..c' < input-06.txt > output.txt 2> stderr.txt"
./ugrep 'a..c' < input-06.txt > output.txt 2> stderr.txt
STATUS=$?
checkResults 06 0

runTest 07 '^123' 0
runTest 08 'wxyz$' 0
runTest 09 'a[bcdef]g' 0
runTest 10 'abc|def|ghi' 0
runTest 11 'ab*c' 0
runTest 12 'ab+c' 0
runTest 13 'ab?c' 0
runTest 14 'a(bc)*d' 0

echo "Test 15: ./ugrep '^Your (license|application|program) has been (revoked|accepted|tested)!$' input-15.txt > output.txt 2> stderr.txt"
./ugrep '^Your (license|application|program) has been (revoked|accepted|tested)!$' input-15.txt > output.txt 2> stderr.txt
STATUS=$?
checkResults 15 0

runTest 16 '[0123456789]+[.][0123456789]+' 0

runTest 17 '*' 1
runTest 18 'abc[123' 1

echo "Test 19: ./ugrep 'abc' not-a-file.txt > output.txt 2> stderr.txt"
./ugrep 'abc' not-a-file.txt > output.txt 2> stderr.txt
STATUS=$?
checkResults 19 1

# Bad command-line arguments
echo "Test 20: ./ugrep too many arguments > output.txt 2> stderr.txt"
./ugrep too many arguments > output.txt 2> stderr.txt
STATUS=$?
checkResults 20 1

runTest 21 'this|that' 1

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi

exit 0
