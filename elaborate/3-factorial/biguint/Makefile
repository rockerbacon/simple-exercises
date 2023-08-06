CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -I.

biguint.so: biguint.c biguint.h
	$(CC) $(CFLAGS) -shared -Ofast -o $@ $<

tests: biguint.c tests.c
	$(CC) $(CFLAGS) -Ofast -o $@ $^

