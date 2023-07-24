#!/bin/bash
# This is a shell script to check your program(s) on test cases.
# It usese the same language you normally use to type commands in
# a terminal window.  You can write whole programs in shell.

# Assume we've succeeded until we see otherwise.
FAIL=0

# Print an error message and set the fail flag.
fail() {
    echo "**** $1"
    FAIL=1
}

# Check the exit status.  The actual exit status (ASTATUS) should match
# the expected status (ESTATUS)
checkStatus() {
    ESTATUS="$1"
    ASTATUS="$2"

    # Make sure the program exited with the right exit status.
    if [ "$ASTATUS" -ne "$ESTATUS" ]; then
        fail "FAILED - incorrect program exit status. (expected $ESTATUS,  Got: $ASTATUS)"
        return 1
    fi

    return 0
}

# Check the contents of an a file.  If the expected file (EFILE)
# exists, then the actual file (AFILE) should exist and it should match.
checkFile() {
    NAME="$1"
    EFILE="$2"
    AFILE="$3"

    # Make sure we're really expecting this file.
    if [ ! -f "$EFILE" ]; then
        return 0
    fi

    # Make sure the output matches the expected output.
    echo "   diff $EFILE $AFILE"
    diff -q "$EFILE" "$AFILE" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
        return 1
    fi

    return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should be empty.
checkFileOrEmpty() {
    NAME="$1"
    EFILE="$2"
    AFILE="$3"

    # if the expected output file doesn't exist, the actual file should be
    # empty.
    if [ ! -f "$EFILE" ]; then
        if [ -s "$AFILE" ]; then
            fail "FAILED - $NAME ($AFILE) should be empty."
            return 1
        fi
        return 0
    fi

    # Make sure the output matches the expected output.
    echo "   diff $EFILE $AFILE"
    diff -q "$EFILE" "$AFILE" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
        return 1
    fi

    return 0
}

# Test the gomoku program
test-gomoku() {
    TESTNO=$1
    ESTATUS=$2

    printf 'Gomoku test %2d\n' "$TESTNO"
    rm -f -- output.txt stderr.txt savegame.gmk

    echo "   ./gomoku ${args[@]} < input/gomoku-${TESTNO}.txt > output.txt 2> stderr.txt"
    ./gomoku "${args[@]}" < "input/gomoku-${TESTNO}.txt" > output.txt 2> stderr.txt
    ASTATUS=$?

    if ! checkStatus "$ESTATUS" "$ASTATUS" ||
        ! checkFileOrEmpty "Stdout output" "output/gomoku-${TESTNO}.txt" "output.txt" ||
        ! checkFileOrEmpty "Stderr output" "output/gomoku-stderr-${TESTNO}.txt" "stderr.txt" ||
        ! checkFileOrEmpty "savegame.gmk" "gmk/gomoku-${TESTNO}.gmk" "savegame.gmk"; then
        FAIL=1
        return 1
    fi

    printf 'Gomoku test %02d PASS\n' "$TESTNO"
    return 0
}

# Test the renju program
test-renju() {
    TESTNO=$1
    ESTATUS=$2

    printf 'Renju test %2d\n' "$TESTNO"
    rm -f -- output.txt stderr.txt savegame.gmk

    echo "   ./renju ${args[@]} < input/renju-${TESTNO}.txt > output.txt 2> stderr.txt"
    ./renju "${args[@]}" < "input/renju-${TESTNO}.txt" > output.txt 2> stderr.txt
    ASTATUS=$?

    if ! checkStatus "$ESTATUS" "$ASTATUS" ||
        ! checkFileOrEmpty "Stdout output" "output/renju-${TESTNO}.txt" "output.txt" ||
        ! checkFileOrEmpty "Stderr output" "output/renju-stderr-${TESTNO}.txt" "stderr.txt" ||
        ! checkFileOrEmpty "savegame.gmk" "gmk/renju-${TESTNO}.gmk" "savegame.gmk"; then
        FAIL=1
        return 1
    fi

    printf 'Renju %02d PASS\n' "$TESTNO"
    return 0
}

# Test the replay program
test-replay() {
    TESTNO=$1
    ESTATUS=$2

    printf 'Replay test %2d\n' "$TESTNO"
    rm -f -- output.txt stderr.txt savegame.gmk

    echo "   ./replay ${args[@]} > output.txt 2> stderr.txt"
    ./replay "${args[@]}" > output.txt 2> stderr.txt
    ASTATUS=$?

    if ! checkStatus "$ESTATUS" "$ASTATUS" ||
        ! checkFileOrEmpty "Stdout output" "output/replay-${TESTNO}.txt" "output.txt" ||
        ! checkFileOrEmpty "Stderr output" "output/replay-stderr-${TESTNO}.txt" "stderr.txt"; then
        FAIL=1
        return 1
    fi

    printf 'Replay test %02d PASS\n' "$TESTNO"
    return 0
}

# make a fresh copy of the target programs
make clean
make
if [ $? -ne 0 ]; then
    fail "Make exited unsuccessfully"
fi

# Run test cases
if [ -x gomoku ]; then
    args=(-o savegame.gmk)
    test-gomoku 1 0
    test-gomoku 2 0
    test-gomoku 3 0
    test-gomoku 4 0
    test-gomoku 5 0
    test-gomoku 6 0
    test-gomoku 7 0
    test-gomoku 8 0
    test-gomoku 9 0
    test-gomoku 10 0
    test-gomoku 11 0
    test-gomoku 12 0
    test-gomoku 13 0
    test-gomoku 14 0
    test-gomoku 15 0
    test-gomoku 16 0
    test-gomoku 17 0
    test-gomoku 18 0
    test-gomoku 19 0
    test-gomoku 20 0
    args=(-b 20)
    test-gomoku 21 1
    args=(-b 19)
    test-gomoku 22 0
    args=(-b -a)
    test-gomoku 23 10
    args=(-r gmk/gomoku-1.gmk)
    test-gomoku 24 0
    args=(-r gmk/gomoku-16.gmk)
    test-gomoku 25 9
    args=(-r gmk/gomoku-26.gmk)
    test-gomoku 26 8
else
    fail "Since your gomoku program didn't compile, we couldn't test it"
fi
if [ -x renju ]; then
    args=(-o savegame.gmk)
    test-renju 1 0
    test-renju 2 0
    test-renju 3 0
    test-renju 4 0
    test-renju 5 0
    test-renju 6 0
    test-renju 7 0
    test-renju 8 0
    test-renju 9 0
    test-renju 10 0
    test-renju 11 0
    test-renju 12 0
    test-renju 13 0
    test-renju 14 0
    test-renju 15 0
    test-renju 16 0
    test-renju 17 0
    test-renju 18 0
    test-renju 19 0
    test-renju 20 0
    args=(-b 20)
    test-renju 21 1
    args=(-b 19)
    test-renju 22 0
    args=(-b -a)
    test-renju 23 10
    args=(-r gmk/renju-1.gmk)
    test-renju 24 0
    args=(-r gmk/renju-11.gmk)
    test-renju 25 9
    args=(-r gmk/renju-16.gmk)
    test-renju 26 9
    args=(-r gmk/renju-27.gmk)
    test-renju 27 8
else
    fail "Since your renju program didn't compile, we couldn't test it"
fi
if [ -x replay ]; then
    gcc -Wall -std=c99 -O3 -D_NOSLEEP io.c board.c game.c replay.c -o replay &> /dev/null
    args=(gmk/gomoku-8.gmk)
    test-replay 1 0
    args=(gmk/gomoku-9.gmk)
    test-replay 2 0
    args=(gmk/gomoku-10.gmk)
    test-replay 3 0
    args=(gmk/gomoku-11.gmk)
    test-replay 4 0
    args=(gmk/gomoku-12.gmk)
    test-replay 5 0
    args=(gmk/gomoku-13.gmk)
    test-replay 6 0
    args=(gmk/gomoku-14.gmk)
    test-replay 7 0
    args=(gmk/gomoku-15.gmk)
    test-replay 8 0
    args=(gmk/gomoku-16.gmk)
    test-replay 9 0
    args=(gmk/gomoku-17.gmk)
    test-replay 10 0
    args=(gmk/renju-8.gmk)
    test-replay 11 0
    args=(gmk/renju-9.gmk)
    test-replay 12 0
    args=(gmk/renju-10.gmk)
    test-replay 13 0
    args=(gmk/renju-11.gmk)
    test-replay 14 0
    args=(gmk/renju-12.gmk)
    test-replay 15 0
    args=(gmk/renju-13.gmk)
    test-replay 16 0
    args=(gmk/renju-14.gmk)
    test-replay 17 0
    args=(gmk/renju-15.gmk)
    test-replay 18 0
    args=(gmk/renju-16.gmk)
    test-replay 19 0
    args=(gmk/renju-17.gmk)
    test-replay 20 0
    args=(gmk/renju-21.gmk)
    test-replay 21 8
    args=(gmk/renju-22.gmk extra)
    test-replay 22 10
else
    fail "Since your replay program didn't compile, we couldn't test it"
fi

if [ $FAIL -ne 0 ]; then
    echo "FAILING TESTS!"
    exit 13
else
    echo "TESTS SUCCESSFUL"
    exit 0
fi
