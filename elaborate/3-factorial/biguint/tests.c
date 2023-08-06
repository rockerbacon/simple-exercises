#include <biguint.h>

biguint_t fixtureA;
biguint_t fixtureB;
biguint_t fixtureC;
biguint_t fixtureD;
biguint_t fixtureE;
biguint_t buffer;

void init_fixtureA(void) {
	biguint_init(&fixtureA);
	biguint_set(&fixtureA, 0);

	fixtureA.value[0] = 0xaabbcc03;
	fixtureA.value[1] = 0x11224469;
	fixtureA.value[2] = 0x4a6a2200;
	fixtureA.value[3] = 0xabcd1200;
}

void init_fixtureB(void) {
	biguint_init(&fixtureB);
	biguint_set(&fixtureB, 0);

	fixtureB.value[0] = 0xc244326e;
	fixtureB.value[1] = 0x03aabbd1;
	fixtureB.value[2] = 0xd5c6b7a8;
	fixtureB.value[3] = 0xa1b2c3d4;
}

void init_fixtureC(void) {
	biguint_init(&fixtureC);
	biguint_set(&fixtureC, 0);

	fixtureC.value[4] = 0xaabbcc03;
	fixtureC.value[5] = 0x11224469;
	fixtureC.value[6] = 0x4a6a2200;
	fixtureC.value[7] = 0xabcd1200;
}

void init_fixtureD(void) {
	biguint_init(&fixtureD);
	biguint_set(&fixtureD, 0);

	fixtureD.value[4] = 0xc244326e;
	fixtureD.value[5] = 0x03aabbd1;
	fixtureD.value[6] = 0xd5c6b7a8;
	fixtureD.value[7] = 0xa1b2c3d4;
}

void init_fixtureE(void) {
	biguint_init(&fixtureE);
	biguint_set(&fixtureE, 0);
	biguint_increase_len(&fixtureE, fixtureE.len * 2);

	fixtureE.value[4] = fixtureD.value[4];
	fixtureE.value[5] = fixtureD.value[5];
	fixtureE.value[6] = fixtureD.value[6];
	fixtureE.value[7] = fixtureD.value[7];
	fixtureE.value[8] = 0x22e34ca9;
	fixtureE.value[9] = 0xa2b34cc1;
	fixtureE.value[10] = 0x52a433ab;
	fixtureE.value[11] = 0xc4c5d2a7;
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
		buffer.value[5] == 0x00 &&
		buffer.value[6] == 0x00 &&
		buffer.value[7] == 0x00 &&
		buffer.len == 8
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
		buffer.len == 16 &&
		buffer.value[0] == 0x00 &&
		buffer.value[1] == 0x00 &&
		buffer.value[2] == 0x00 &&
		buffer.value[3] == 0x00 &&
		buffer.value[4] == 0x6cfffe71 &&
		buffer.value[5] == 0x14cd003b &&
		buffer.value[6] == 0x2030d9a8 &&
		buffer.value[7] == 0x4d7fd5d5 &&
		buffer.value[8] == 0x00000001 &&
		buffer.value[9] == 0x00 &&
		buffer.value[10] == 0x00 &&
		buffer.value[11] == 0x00 &&
		buffer.value[12] == 0x00 &&
		buffer.value[13] == 0x00 &&
		buffer.value[14] == 0x00 &&
		buffer.value[15] == 0x00
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
		buffer.len == 16 &&
		buffer.value[0] == 0x00 &&
		buffer.value[1] == 0x00 &&
		buffer.value[2] == 0x00 &&
		buffer.value[3] == 0x00 &&
		buffer.value[4] == 0x6cfffe71 &&
		buffer.value[5] == 0x14cd003b &&
		buffer.value[6] == 0x2030d9a8 &&
		buffer.value[7] == 0x4d7fd5d5 &&
		buffer.value[8] == 0x22e34caa &&
		buffer.value[9] == 0xa2b34cc1 &&
		buffer.value[10] == 0x52a433ab &&
		buffer.value[11] == 0xc4c5d2a7 &&
		buffer.value[12] == 0x00 &&
		buffer.value[13] == 0x00 &&
		buffer.value[14] == 0x00 &&
		buffer.value[15] == 0x00
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
		buffer.len == 16 &&
		buffer.value[0] == 0x00 &&
		buffer.value[1] == 0x00 &&
		buffer.value[2] == 0x00 &&
		buffer.value[3] == 0x00 &&
		buffer.value[4] == 0x6cfffe71 &&
		buffer.value[5] == 0x14cd003b &&
		buffer.value[6] == 0x2030d9a8 &&
		buffer.value[7] == 0x4d7fd5d5 &&
		buffer.value[8] == 0x22e34caa &&
		buffer.value[9] == 0xa2b34cc1 &&
		buffer.value[10] == 0x52a433ab &&
		buffer.value[11] == 0xc4c5d2a7 &&
		buffer.value[12] == 0x00 &&
		buffer.value[13] == 0x00 &&
		buffer.value[14] == 0x00 &&
		buffer.value[15] == 0x00
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

	biguint_destroy(&fixtureA);
	biguint_destroy(&fixtureB);
	biguint_destroy(&fixtureC);
	biguint_destroy(&fixtureD);
	biguint_destroy(&fixtureE);
	biguint_destroy(&buffer);

	return failures > 0;
}
