#!/bin/sh -e

make cleanup
make assert=no opt=no cov=yes
make assert=no opt=no cov=yes test

RCM_HEADER=`ls src/rcm_*.h`

for HEADER in $RCM_HEADER; do
  FILE=`echo $HEADER | sed s/.h$/.c/`
  OBJF=`echo $FILE | sed s/.c$/.o/ | sed s%src/%obj/%`
  gcov -o $OBJF $FILE
done

for HEADER in $RCM_HEADER; do
  GCOV=`echo $HEADER | sed s/.h$/.c.gcov/ | sed s%src/%%`
  echo "checking full coverage for $GCOV"
  ! grep "^    #####" $GCOV || exit 1
done
