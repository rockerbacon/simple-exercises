CC=gcc
AR=ar
CFLAGS=-Wall -Wextra -Werror -pedantic -I.

default: tests libbiguint.a

biguint.o: biguint.c biguint.h
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

libbiguint.a: biguint.o
	$(AR) rcs $@ $^

tests: biguint.c tests.c
	$(CC) $(CFLAGS) -Ofast -o $@ $^

