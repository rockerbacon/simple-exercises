#!/bin/env bash

set -e

function print_help() {
cat <<EOF
Usage: $0 [OPTIONS...] IMPLEMENTATION IMPLEMENTATION [IMPLEMENTATION]

IMPLEMENTATION: implementation to compare. Can be: c, go or python
OPTIONS:
	--max,-a	the largest factorial to check. Defaults to 100
	--min,-i	the smallest factorial to check. Defaults to 0
	--help,-h	print this help message and exit
EOF
}

function read_num_value() {
	arg=$1
	value=$2

	num_regexp='^[0-9]+$'
	if ! [[ $value =~ $num_regexp ]]; then
		echo "ERROR: Invalid value '$value' for option '$arg'" >&2
		print_help >&2
		exit 1
	fi

	echo "$value"
}

implementations=()
max=100
min=0
function parse_args() {
	while [ -n "$1" ]; do
		case "$1" in
			--help|-h)
				print_help
				exit 0
			;;
			--max|-a)
				max=$(read_num_value "$1" "$2")
				shift
			;;
			--min|-i)
				min=$(read_num_value "$1" "$2")
				shift
			;;
			-*)
				echo "ERROR: Unknown option '$1'" >&2
				print_help
				exit 1
			;;
			c|go|python)
				implementations+=("$1")
			;;
			*)
				echo "ERROR: Invalid IMPLEMENTATION '$1'" >&2
				print_help
				exit 1
			;;
		esac
		shift
	done

	if [ "${#implementations[@]}" -lt "2" ]; then
		echo "ERROR: Not enough implementations were specified" >&2
		print_help
		exit 1
	fi
}

parse_args "$@"

set -u

storage=/tmp/factorial_consistency_check

mkdir -p "$storage"
for impl in "${implementations[@]}"; do
	executable="invalid_impl"
	case "$impl" in
		c) executable="./c/main";;
		go) executable="./go/simple_exercises";;
		python) executable="./python/main.py";;
	esac

	echo "Calculating $min! to $max! using the $impl implementation..."
	for i in $(seq $min $max); do
		"$executable" "$i" > "$storage/${impl}_${i}.txt"
	done
done

echo "Checking whether all calculated values are consistent..."
for i in $(seq $min $max); do
	reference_impl=${implementations[0]}
	reference_result="$storage/${reference_impl}_${i}.txt"
	for impl in "${implementations[@]:1}"; do
		impl_result="$storage/${impl}_${i}.txt"
		if ! diff -q "$reference_result" "$impl_result" >/dev/null; then
			echo "Differences encountered between implementations '${reference_impl}' and '${impl}' when calculating value '${i}'"
			rm -rf "$storage"
			exit 1
		fi
	done
done

echo "All implementations are consistent for values from $min! to $max!"
rm -rf "$storage"

