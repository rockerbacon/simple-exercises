#!/bin/env python3

import sys

def calculate_final_capital(initial_capital, interest_rate, investment_time):
    monthly_factor = interest_rate / 100 + 1
    return round(monthly_factor ** investment_time * initial_capital, 2)

def print_help():
    print(f'Usage: {sys.argv[0]} [INITIAL_CAPITAL] [INTEREST_RATE] [INVESTMENT_TIME]')
    print()
    print('INITIAL_CAPITAL: initial monetary amount invested. eg.: 10234.15')
    print('INTEREST_RATE: monthly interest rate of the investment (in percentage). eg.: 7.15')
    print('INVESTMENT_TIME: duration of the investment (in months). eg.: 24')

def main():
    if (len(sys.argv) < 4 or sys.argv[1] == '--help' or sys.argv[1] == '-h'):
        print_help()
        return

    initial_capital = float(sys.argv[1])
    interest_rate = float(sys.argv[2])
    investment_time = int(sys.argv[3])
    final_capital = calculate_final_capital(
        initial_capital,
        interest_rate,
        investment_time
    )

    print(f'Capital after {investment_time} months: {final_capital}')

if __name__ == '__main__':
    main()

