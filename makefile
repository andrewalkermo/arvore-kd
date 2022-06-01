all: clean main clean

main:
	gcc -o main src/main.c && ./main

clean:
	rm -f main

test:
	sh ./tests/run_tests.sh