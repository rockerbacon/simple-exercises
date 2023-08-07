#include <biguint.h>
#include <stdio.h>
#include <string.h>

#define OPTION_HELP 1
#define OPTION_QUIET 2

typedef struct {
	int options;
	uint32_t number;
} arguments_t;

void print_help(const char* binary_path) {
	printf("Usage: %s [OPTIONS...] NUMBER\n\n", binary_path);
	printf("NUMBER:\tany positive integer\n");
	printf("OPTIONS:\n");
	printf("\t--help,-h\tprint this help message and exit\n");
	printf("\t--quiet,-q\tdon't print the factorial result\n\n");
}

int check_is_integer(const char* str) {
	for (size_t i = 0; str[i] != '\0'; i++) {
		if (str[i] > '9' || str[i] < '0') {
			return 0;
		}
	}

	return 1;
}

int parse_args(arguments_t* args, int argc, char** argv) {
	int is_reading_options = 1;

	args->options = 0;

	int i = 1;
	while (is_reading_options && i < argc) {
		char* arg = argv[i];

		if (arg[0] != '-') {
			is_reading_options = 0;
		} else {
			if (
				strcmp(arg, "--help") == 0 ||
				strcmp(arg, "-h") == 0
			) {
				args->options |= OPTION_HELP;
				return 0;
			} else if (
				strcmp(arg, "--quiet") == 0 ||
				strcmp(arg, "-q") == 0
			) {
				args->options |= OPTION_QUIET;
			} else if (
				strcmp(arg, "--") == 0
			) {
				is_reading_options = 0;
			} else {
				fprintf(stderr, "ERROR: Unknown option '%s'\n", arg);
				return 1;
			}

			i++;
		}
	}

	if (i >= argc) {
		fprintf(stderr, "ERROR: Missing NUMBER argument\n");
		return 1;
	}

	if (!check_is_integer(argv[i])) {
		fprintf(stderr, "ERROR: Invalid NUMBER argument '%s'\n", argv[i]);
		return 1;
	}

	args->number = (uint32_t)atoi(argv[i]);

	return 0;
}

int main(int argc, char** argv) {
	arguments_t args;
	biguint_t result;

	int parsing_error = parse_args(&args, argc, argv);

	if (parsing_error || args.options & OPTION_HELP) {
		print_help(argv[0]);
		return parsing_error;
	}

	biguint_init(&result);
	biguint_factorial(&result, args.number);

	if (!(args.options & OPTION_QUIET)) {
		biguint_print_base16(stdout, &result);
		fprintf(stdout, "\n");
	}

	biguint_destroy(&result);
	return 0;
}

