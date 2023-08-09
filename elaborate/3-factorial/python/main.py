#!/bin/env python3

import math
import argparse

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-q', '--quiet',
        action='store_true',
        default=False,
        help='don\'t print the factorial result'
    )
    parser.add_argument(
        'NUMBER',
        type=int,
        help='any positive integer'
    )

    return parser.parse_args()

def main():
    args = parse_args()

    result = math.factorial(args.NUMBER)

    if not args.quiet:
        print(hex(result))

if __name__ == '__main__':
    main()

