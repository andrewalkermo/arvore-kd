#!/bin/bash
result=0
cp -r src/ tests/src_temp/
for input in tests/duas_partes/*.in; do
  if [[ ${input: -1} == 'a' ]]; then
    rm -f arvore.dat
  fi
  output=${input%.in}.out
  gcc -o main tests/src_temp/main.c
  ./main < $input > test.out
  diff -w -u1 --color=auto --palette='ad=1;3;38;5;154:de=1;3;38;5;9' test.out $output && echo "OK" || result=1
done
rm -f test.out
rm -f main
rm -rf tests/src_temp/
exit $result