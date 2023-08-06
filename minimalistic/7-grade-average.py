#!/bin/env python3

import sys

def calculate_average(grades):
    return round(sum(grades) / len(grades), 2)

def print_help():
    print(f'Usage: {sys.argv[0]} [GRADE...]')
    print()
    print('GRADE: any positive number. eg.: 3.14')

def main():
    if (len(sys.argv) < 2 or sys.argv[1] == '--help' or sys.argv[1] == '-h'):
        print_help()
        return

    average = calculate_average([float(x) for x in sys.argv[1:]])
    print(f'Average: {average}')

if __name__ == '__main__':
    main()

