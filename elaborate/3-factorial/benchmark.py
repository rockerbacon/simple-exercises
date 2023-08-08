#!/bin/env python3

import argparse
import os
import subprocess
import time

BENCH_NUMBERS = [10, 100, 1_000, 10_000, 100_000, 1_000_000]

IMPLEMENTATION = {
    'c': os.path.join('c', 'main'),
    'python': os.path.join('python', 'main.py'),
    'go': os.path.join('go', 'simple_exercises'),
}

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--runs', '-r',
        default=5,
        type=int,
        help='Number of runs for each benchmark. Defaults to 5'
    )
    parser.add_argument(
        'implementations',
        nargs='+',
        choices=IMPLEMENTATION.keys(),
        help='Implementations to benchmark'
    )

    return parser.parse_args()

def bench(implementation_id, runs):
    summary = []
    for number in BENCH_NUMBERS:
        total_time = 0
        for i in range(0, runs):
            start_time = time.perf_counter_ns()
            execution = subprocess.run(
                [IMPLEMENTATION[implementation_id], '--quiet', str(number)],
                check=True
            )
            total_time += time.perf_counter_ns() - start_time

        avg_time = total_time // runs

        summary.append({
            'number': number,
            'avg': avg_time,
        })

    return summary

def format_table_row(col1, col2, col3):
    return '|{:^20}|{:^15}|{:^15}|'.format(col1, col2, col3)

def print_table_header():
    print(format_table_row('Implementation', 'Calculation', 'Time'))

def print_table_row(implementation, calculation, time):
    print(format_table_row(implementation, f'{calculation}!', f'{time // 1000000}ms'))

def main():
    args = parse_args()

    print('Running benchmarks. This may take a while...')
    print()
    print_table_header()
    for implementation in args.implementations:
        summary = bench(implementation, args.runs)
        for entry in summary:
            print_table_row(
                implementation,
                entry['number'],
                entry['avg']
            )

    print()

if __name__ == '__main__':
    main()

