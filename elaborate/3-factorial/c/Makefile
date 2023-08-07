CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic
LFLAGS=-I./biguint -L./biguint -lbiguint

default: main

.PHONY: clean

biguint/libbiguint.a:
	make --directory biguint

main: main.c biguint/libbiguint.a
	$(CC) $(CFLAGS) -Ofast -o $@ $^ $(LFLAGS)

clean:
	rm -f main
	make --directory biguint clean

