#!/bin/env python3

import sys

def print_help():
    print(f'Usage: {sys.argv[0]} [WORD]')
    print()
    print('WORD: any possible word. eg.: pineapple')

def is_palindrome(word):
    lower_word = word.lower()
    for i in range(len(lower_word) // 2):
        if lower_word[i] != lower_word[-i-1]:
            return False

    return True

def main():
    if (len(sys.argv) < 2 or sys.argv[1] == '--help' or sys.argv[1] == '-h'):
        print_help()
        return

    if is_palindrome(sys.argv[1]):
        print('It IS a palindrome!')
    else:
        print('It is NOT a palindrome')

if __name__ == '__main__':
    main()

