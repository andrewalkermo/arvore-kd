#!/bin/bash
result=0
cp -r src/ tests/src_temp/
for input in tests/*.in; do
  output=${input%.in}.out
  rm -f arvore.dat
  gcc -std=c99 -Wall -o main tests/src_temp/main.c
  ./main < $input > test.out
  diff -w -u1 --color=auto --palette='ad=1;3;38;5;154:de=1;3;38;5;9' test.out $output && echo "OK" || result=1
done
rm -f arvore.dat
rm -f test.out
rm -f main
rm -rf tests/src_temp/
exit $result