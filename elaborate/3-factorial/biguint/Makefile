CC=gcc
AR=ar
CFLAGS=-Wall -Wextra -Werror -pedantic -I.

default: tests libbiguint.a

factorial.o: factorial.c biguint.h
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

biguint.o: biguint.c biguint.h
	$(CC) $(CFLAGS) -Ofast -c -o $@ $<

libbiguint.a: biguint.o factorial.o
	$(AR) rcs $@ $^

tests: tests.c libbiguint.a
	$(CC) $(CFLAGS) -Ofast -o $@ $^ -L. -lbiguint

