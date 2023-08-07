package main

import (
	"errors"
	"flag"
	"fmt"
	"os"
	"simple_exercises/bigmath"
	"strconv"
)

type Arguments struct {
	quiet bool;
	number uint32;
}

func overrideFlagHelpMessage() {
	flag.Usage = func() {
		fmt.Printf("Usage: %s [OPTIONS...] NUMBER\n\n", os.Args[0])
		fmt.Printf("NUMBER:\tany possible integer\n")
		fmt.Printf("OPTIONS:\n")
		flag.PrintDefaults()
	}
}

func parse_args() (*Arguments, error) {
	var args Arguments

	overrideFlagHelpMessage();

	flag.BoolVar(&args.quiet, "quiet", false, "don't print the factorial result")
	flag.Parse();

	numberStr := flag.Arg(0)
	if (numberStr == "") {
		return nil, errors.New("Missing NUMBER argument")
	}

	number, numberParsingErr := strconv.ParseUint(numberStr, 10, 32)

	if (numberParsingErr != nil) {
		return nil, errors.New(fmt.Sprintf("Invalid NUMBER argument '%s'", numberStr))
	}

	args.number = uint32(number)

	return &args, nil
}

func main() {
	args, argsErr := parse_args()

	if (argsErr != nil) {
		fmt.Println(argsErr.Error())
		flag.Usage()
		os.Exit(1);
	}

	result := bigmath.Factorial(args.number)

	if (!args.quiet) {
		fmt.Printf("0x%s\n", result.Text(16))
	}
}

