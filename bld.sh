#!/bin/sh


OPTS=""
if echo "$OSTYPE" | egrep -i linux >/dev/null; then :
  OPTS="-l rt"
fi

gcc -Wall -o hist_test $OPTS rdtsc.c hist.c hist_test.c
if [ $? -ne 0 ]; then exit 1; fi
