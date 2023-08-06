#!/bin/env python3

import sys

def count_vowels(phrase):
    vowels = {
        'a': 1,
        'e': 1,
        'i': 1,
        'o': 1,
        'u': 1
    }

    count = 0
    for letter in phrase:
        count += vowels.get(letter.lower(), 0)

    return count

def print_help():
    print(f'Usage: {sys.argv[0]} [PHRASE...]')
    print()
    print('PHRASE: any ASCII encoded phrase')

def main():
    if (len(sys.argv) < 2 or sys.argv[1] == '--help' or sys.argv[1] == '-h'):
        print_help()
        return

    vowels_count = count_vowels(''.join(sys.argv[1:]))
    print(f'This phrase has {vowels_count} vowels')

if __name__ == '__main__':
    main()

