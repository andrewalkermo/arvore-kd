all: clean main clean

main:
	gcc -o main src/main.c && ./main

clean:
	rm -f main

test:
	sh ./tests/test_main.sh