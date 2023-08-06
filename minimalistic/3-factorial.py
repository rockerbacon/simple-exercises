#!/bin/env python3

import sys

def factorial(value):
    if (value < 2):
        return 1

    return value * factorial(value - 1)

def print_help():
    print(f'Usage: {sys.argv[0]} [NUMBER]')
    print()
    print('NUMBER: any positive integer. eg.: 3')

def main():
    if (len(sys.argv) < 2 or sys.argv[1] == '--help' or sys.argv[1] == '-h'):
        print_help()
        return

    print(factorial(int(sys.argv[1])))

if __name__ == '__main__':
    main()

