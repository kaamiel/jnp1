#!/bin/bash

g++ -Wall -Wextra -pedantic -O3 -std=c++17 tst_test1.cc -o out
./out > tst_test1.out2
echo -ne "tst_test1.cc\t"
if diff tst_test1.out tst_test1.out2 > /dev/null 2>&1
then
  echo -e "\033[0;32mpassed\033[0m"
else
  echo -e "\033[0;31mfailed\033[0m"
fi


cp tst.h ./tests/

for f in tests/tst_test*.cc
do
  g++ -Wall -Wextra -pedantic -O3 -std=c++17 $f -o out
  echo -ne "$f\t"
  if ./out > /dev/null 2>&1
  then
    echo -e "\033[0;32mpassed\033[0m"
  else
    echo -e "\033[0;31mfailed\033[0m"
  fi
done

rm -f out tst_test1.out2

