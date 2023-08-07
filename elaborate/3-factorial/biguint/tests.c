#include <biguint.h>

biguint_t fixtureA;
biguint_t fixtureB;
biguint_t fixtureC;
biguint_t fixtureD;
biguint_t fixtureE;
biguint_t buffer;

void set_range(biguint_t* biguint, size_t range_start, size_t range_end, uint32_t digit) {
	for (size_t i = range_start; i < range_end; i++) {
		biguint->value[i] = digit;
	}
}

void init_fixtureA(void) {
	biguint_init(&fixtureA);

	fixtureA.value[0] = 0xaabbcc03;
	fixtureA.value[1] = 0x11224469;
	fixtureA.value[2] = 0x4a6a2200;
	fixtureA.value[3] = 0xabcd1200;

	fixtureA.len = 4;
}

void init_fixtureB(void) {
	biguint_init(&fixtureB);

	fixtureB.value[0] = 0xc244326e;
	fixtureB.value[1] = 0x03aabbd1;
	fixtureB.value[2] = 0xd5c6b7a8;
	fixtureB.value[3] = 0xa1b2c3d4;

	fixtureB.len = 4;
}

void init_fixtureC(void) {
	biguint_init(&fixtureC);
	set_range(&fixtureC, 0, 4, 0);

	fixtureC.value[4] = 0xaabbcc03;
	fixtureC.value[5] = 0x11224469;
	fixtureC.value[6] = 0x4a6a2200;
	fixtureC.value[7] = 0xabcd1200;

	fixtureC.len = 8;
}

void init_fixtureD(void) {
	biguint_init(&fixtureD);
	set_range(&fixtureD, 0, 4, 0);

	fixtureD.value[4] = 0xc244326e;
	fixtureD.value[5] = 0x03aabbd1;
	fixtureD.value[6] = 0xd5c6b7a8;
	fixtureD.value[7] = 0xa1b2c3d4;

	fixtureD.len = 8;
}

void init_fixtureE(void) {
	biguint_init(&fixtureE);
	set_range(&fixtureE, 0, 4, 0);

	fixtureE.value[4] = fixtureD.value[4];
	fixtureE.value[5] = fixtureD.value[5];
	fixtureE.value[6] = fixtureD.value[6];
	fixtureE.value[7] = fixtureD.value[7];
	fixtureE.value[8] = 0x22e34ca9;
	fixtureE.value[9] = 0xa2b34cc1;
	fixtureE.value[10] = 0x52a433ab;
	fixtureE.value[11] = 0xc4c5d2a7;

	fixtureE.len = 12;
}

int is_all_zero(biguint_t* biguint, size_t range_start, size_t range_end) {
	for (size_t i = range_start; i < range_end; i++) {
		if (biguint->value[i] != 0) {
			return 0;
		}
	}

	return 1;
}

void print_operation(biguint_t* operandA, char operator, biguint_t* operandB, biguint_t* result) {
	fprintf(stdout, "\t");
	biguint_print_base16(stdout, operandA);
	fprintf(stdout, "\n\t %c \n\t", operator);
	biguint_print_base16(stdout, operandB);
	fprintf(stdout, "\n\t = \n\t");
	biguint_print_base16(stdout, result);
	fprintf(stdout, "\n");
}

int test_sum_without_overflow(void) {
	fprintf(stdout, "Testing biguint_sum(), without overflows...\n");

	biguint_sum(&buffer, &fixtureA, &fixtureB);

	print_operation(&fixtureA, '+', &fixtureB, &buffer);

	int failure;
	if (
		buffer.value[0] == 0x6cfffe71 &&
		buffer.value[1] == 0x14cd003b &&
		buffer.value[2] == 0x2030d9a8 &&
		buffer.value[3] == 0x4d7fd5d5 &&
		buffer.value[4] == 0x00000001 &&
		buffer.len == 5
	) {
		failure = 0;
		fprintf(stdout, "Passed.\n\n");
	} else {
		failure = 1;
		fprintf(stdout, "!!!FAILED!!!\n\n");
	}

	return failure;
}

int test_sum_with_overflow(void) {
	fprintf(stdout, "Testing biguint_sum(), with overflow...\n");

	biguint_sum(&buffer, &fixtureC, &fixtureD);

	print_operation(&fixtureC, '+', &fixtureD, &buffer);

	int failure;
	if (
		is_all_zero(&buffer, 0, 4) &&
		buffer.value[4] == 0x6cfffe71 &&
		buffer.value[5] == 0x14cd003b &&
		buffer.value[6] == 0x2030d9a8 &&
		buffer.value[7] == 0x4d7fd5d5 &&
		buffer.value[8] == 0x00000001 &&
		buffer.len == 9
	) {
		failure = 0;
		fprintf(stdout, "Passed.\n\n");
	} else {
		failure = 1;
		fprintf(stdout, "!!!FAILED!!!\n\n");
	}

	return failure;
}

int test_sum_with_second_addend_having_greater_length(void) {
	fprintf(stdout, "Testing biguint_sum(), with second addend having greater length...\n");

	biguint_sum(&buffer, &fixtureC, &fixtureE);

	print_operation(&fixtureC, '+', &fixtureE, &buffer);

	int failure;
	if (
		is_all_zero(&buffer, 0, 4) &&
		buffer.value[4] == 0x6cfffe71 &&
		buffer.value[5] == 0x14cd003b &&
		buffer.value[6] == 0x2030d9a8 &&
		buffer.value[7] == 0x4d7fd5d5 &&
		buffer.value[8] == 0x22e34caa &&
		buffer.value[9] == 0xa2b34cc1 &&
		buffer.value[10] == 0x52a433ab &&
		buffer.value[11] == 0xc4c5d2a7 &&
		buffer.len == 12
	) {
		failure = 0;
		fprintf(stdout, "Passed.\n\n");
	} else {
		failure = 1;
		fprintf(stdout, "!!!FAILED!!!\n\n");
	}

	return failure;
}

int test_sum_with_first_addend_having_greater_length(void) {
	fprintf(stdout, "Testing biguint_sum(), with first addend having greater length...\n");

	biguint_sum(&buffer, &fixtureE, &fixtureC);

	print_operation(&fixtureE, '+', &fixtureC, &buffer);

	int failure;
	if (
		is_all_zero(&buffer, 0, 4) &&
		buffer.value[4] == 0x6cfffe71 &&
		buffer.value[5] == 0x14cd003b &&
		buffer.value[6] == 0x2030d9a8 &&
		buffer.value[7] == 0x4d7fd5d5 &&
		buffer.value[8] == 0x22e34caa &&
		buffer.value[9] == 0xa2b34cc1 &&
		buffer.value[10] == 0x52a433ab &&
		buffer.value[11] == 0xc4c5d2a7 &&
		buffer.len == 12
	) {
		failure = 0;
		fprintf(stdout, "Passed.\n\n");
	} else {
		failure = 1;
		fprintf(stdout, "!!!FAILED!!!\n\n");
	}

	return failure;
}

int test_mult(void) {
	fprintf(stdout, "Testing biguint_mult()...\n");

	biguint_mult(&buffer, &fixtureC, &fixtureD);

	print_operation(&fixtureC, '*', &fixtureD, &buffer);

	int failure;
	if (
		is_all_zero(&buffer, 0, 8) &&
		buffer.value[8] == 0x81563f4a &&
		buffer.value[9] == 0x61df72af &&
		buffer.value[10] == 0x0cc3c95f &&
		buffer.value[11] == 0x7b028a9d &&
		buffer.value[12] == 0xe49ad395 &&
		buffer.value[13] == 0xa9a88927 &&
		buffer.value[14] == 0xbd0099f0 &&
		buffer.value[15] == 0x6c83f054 &&
		buffer.len == 16
	) {
		failure = 0;
		fprintf(stdout, "Passed.\n\n");
	} else {
		failure = 1;
		fprintf(stdout, "!!!FAILED!!!\n\n");
	}

	return failure;
}


int main(void) {
	int failures = 0;

	init_fixtureA();
	init_fixtureB();
	init_fixtureC();
	init_fixtureD();
	init_fixtureE();
	biguint_init(&buffer);

	failures += test_sum_without_overflow();
	failures += test_sum_with_overflow();
	failures += test_sum_with_second_addend_having_greater_length();
	failures += test_sum_with_first_addend_having_greater_length();
	failures += test_mult();

	biguint_destroy(&fixtureA);
	biguint_destroy(&fixtureB);
	biguint_destroy(&fixtureC);
	biguint_destroy(&fixtureD);
	biguint_destroy(&fixtureE);
	biguint_destroy(&buffer);

	if (failures > 0) {
		fprintf(stdout, "!!!%d tests FAILED!!!\n", failures);
		return 1;
	}

	fprintf(stdout, "All tests passed!\n");
	return 0;
}
