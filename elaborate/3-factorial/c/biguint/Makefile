CC=gcc
AR=ar
CFLAGS=-Wall -Wextra -Werror -pedantic -I.

default: libbiguint.a

.PHONY: clean

factorial.o: factorial.c biguint.h
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

biguint.o: biguint.c biguint.h
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

libbiguint.a: biguint.o factorial.o
	$(AR) rcs $@ $^

tests: tests.c libbiguint.a
	$(CC) $(CFLAGS) -Ofast -o $@ $^ -L. -lbiguint

clean:
	rm -f libbiguint.a biguint.o factorial.o tests

