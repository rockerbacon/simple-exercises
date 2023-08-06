#!/bin/env python3

def maximum_possible_divisor(value):
    return value // 2

def is_prime(value):
    divisor = maximum_possible_divisor(value)
    while divisor > 1:
        if value % divisor == 0:
            return False
        divisor -= 1

    return True

def primes(count):
    candidate = 1
    while count > 0:
        if is_prime(candidate):
            yield candidate
            count -= 1
        candidate += 1

def main():
    for prime in primes(10):
        print(prime)

if __name__ == "__main__":
    main()

