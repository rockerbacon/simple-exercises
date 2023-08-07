#!/bin/env python3

import sys
import math

OPTION_HELP = 1
OPTION_QUIET = 2

def factorial(value):
    if (value < 2):
        return 1

    return value * factorial(value - 1)

def print_help():
    print(f'Usage: {sys.argv[0]} [OPTIONS...] NUMBER')
    print()
    print('NUMBER: any positive integer')
    print('OPTIONS:')
    print('\t--help,-h\tprint this help message and exit')
    print('\t---quiet,-q\tdon\'t print the factorial result')

def parse_args():
    args = {
        'options': 0,
        'number': 0,
    }

    is_reading_options = True
    i = 1
    while is_reading_options and i < len(sys.argv):
        arg = sys.argv[i]

        if arg[0] != '-':
            is_reading_options = False
        else:
            if arg == '--help' or arg == '-h':
                args['options'] |= OPTION_HELP
                return args
            elif arg == '--quiet' or arg == '-q':
                args['options'] |= OPTION_QUIET
            elif arg == '--':
                is_reading_options = False
            else:
                raise Exception(f"ERROR: Unknown option '{arg}'")
            i += 1

    if i >= len(sys.argv):
        raise Exception("ERROR: Missing NUMBER argument")

    try:
        args['number'] = int(sys.argv[i])
    except:
        raise Exception(f"ERROR: Invalid NUMBER argument '{sys.argv[i]}'")

    return args

def main():
    args = None
    try:
        args = parse_args()
    except Exception as e:
        print(e)
        print_help()
        return 1

    if args['options'] & OPTION_HELP:
        print_help()
        return 0

    result = math.factorial(args['number'])

    if not (args['options'] & OPTION_QUIET):
        print(hex(result))

    return 0

if __name__ == '__main__':
    main()

