#!/bin/env python3

import sys

def calculate(expression):
    operators = {
        '+': lambda a, b: a + b,
        '-': lambda a, b: a - b,
        '*': lambda a, b: a * b,
        '/': lambda a, b: a / b
    }

    return operators[expression['operator']](
        expression['first_operand'],
        expression['second_operand']
    )

def parse_expression(tokens):
    return {
        'first_operand': float(tokens[0]),
        'second_operand': float(tokens[1]),
        'operator': tokens[2],
    }

def print_help():
    print(f'Usage: {sys.argv[0]} [FIRST_OPERAND] [SECOND_OPERAND] [OPERATOR]')
    print()
    print('FIRST_OPERAND: any valid number. eg.: 3.14')
    print('SECOND_OPERAND: any valid number. eg.: 3.14')
    print('OPERATOR: any of the 4 elementary operators: +, -, *, /')
    print('\tRemember to escape the multiplication operator to prevent it from being expanded by your shell. eg.: \* or "*"')

def main():
    if (len(sys.argv) < 2 or sys.argv[1] == "--help" or sys.argv[1] == "-h"):
        print_help()
        return

    result = calculate(parse_expression(sys.argv[1:]))

    print("Result: ", result)

if __name__ == "__main__":
    main()

