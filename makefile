all: clean main clean

main:
	gcc src/utils/utils.c src/arvore_obras/arvore_obras.c src/consultas/consultas.c src/operacoes_arquivo/operacoes_arquivo.c src/main.c -std=c99 -Wall -o main && ./main

clean:
	rm -f main
	rm -f arvore.dat

test:
	sh ./tests/run_tests.sh
