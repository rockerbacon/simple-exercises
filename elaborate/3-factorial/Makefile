CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic
LFLAGS=-I./biguint -L./biguint -lbiguint

default: main

main: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

