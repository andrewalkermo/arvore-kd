#!/bin/bash
result=0
cp -r src/ tests/src_temp/
sed -i 's/#define NREGSPORPAGINA 3/##define NREGSPORPAGINA 1/' tests/src_temp/main.c

for input in tests/*.in; do
  output=${input%.in}.out
  rm -f arvore.dat
  gcc -o main tests/src_temp/main.c
  ./main < $input > test.out
  diff -w -u1 --color=auto --palette='ad=1;3;38;5;154:de=1;3;38;5;9' test.out $output && echo "OK" || result=1
done

rm -f arvore.dat
rm -f test.out
rm -f main
rm -rf tests/src_temp/
exit $result