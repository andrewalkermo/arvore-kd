#!/bin/bash
result=0
cp -r src/ tests/src_temp/

for input in tests/teste_entradas_saidas/entradas/*; do
  echo "Testando $input"
  if [[ ${input: -1} == 'a' ]]; then
    rm -f arvore.dat
  fi

  output=tests/teste_entradas_saidas/saidas/s${input##*/teste}
  
  gcc tests/src_temp/utils/utils.c tests/src_temp/arvore_obras/arvore_obras.c tests/src_temp/consultas/consultas.c tests/src_temp/operacoes_arquivo/operacoes_arquivo.c tests/src_temp/main.c -std=c99 -Wall -o main
  ./main < $input > test.out
  diff -w -u1 --color=auto --palette='ad=1;3;38;5;154:de=1;3;38;5;9' test.out $output && echo "OK" || result=1

done

rm -f test.out
rm -f main
rm -rf tests/src_temp/

exit $result
