#!/bin/env python3

import sys

def multiplication_table(number):
    for i in range(1, 11):
        yield {
            'multiplier': i,
            'result': i * number
        }

def print_help():
    print(f'Usage: {sys.argv[0]} [NUMBER]')
    print()
    print('NUMBER: integer in the interval [1, 10]')

def main():
    if (len(sys.argv) < 2 or sys.argv[1] == '--help' or sys.argv[1] == '-h'):
        print_help()
        return

    number = int(sys.argv[1])
    for row in multiplication_table(number):
        print(f'{row["multiplier"]}x{number} = {row["result"]}')

if __name__ == '__main__':
    main()

