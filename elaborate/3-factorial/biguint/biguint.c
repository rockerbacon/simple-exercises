#include <biguint.h>
#include <stdlib.h>

void biguint_init(biguint_t* biguint) {
	biguint->len = BIGUINT_INIT_LEN;
	biguint->value = (uint32_t*)malloc(BIGUINT_INIT_LEN*sizeof(uint32_t));
}

void biguint_destroy(biguint_t* biguint) {
	free(biguint->value);
}

void biguint_increase_len(biguint_t* biguint, size_t new_len) {
	uint32_t* value_copy = (uint32_t*)malloc(new_len*sizeof(uint32_t));

	for (size_t i = 0; i < biguint->len; i++) {
		value_copy[i] = biguint->value[i];
	}

	for (size_t i = biguint->len; i < new_len; i++) {
		value_copy[i] = 0;
	}

	biguint_destroy(biguint);
	biguint->len = new_len;
	biguint->value = value_copy;
}

void biguint_set(biguint_t* biguint, uint64_t value) {
	biguint->value[0] = value;
	for (size_t i = 1; i < biguint->len; i++) {
		biguint->value[i] = 0;
	}
}

void biguint_sum(biguint_t* total_buffer, biguint_t* addend1, biguint_t* addend2) {
	biguint_t* greater_addend;
	biguint_t* smaller_addend;

	if (addend1->len < addend2->len) {
		greater_addend = addend2;
		smaller_addend = addend1;
	} else {
		greater_addend = addend1;
		smaller_addend = addend2;
	}

	if (total_buffer->len < greater_addend->len) {
		biguint_increase_len(total_buffer, greater_addend->len + 1);
	}

	size_t i = 0;
	uint32_t carry = 0;
	while (i < smaller_addend->len) {
		uint64_t sum =
			(uint64_t)smaller_addend->value[i] +
			(uint64_t)greater_addend->value[i] +
			(uint64_t)carry;

		total_buffer->value[i] = (uint32_t)sum;
		carry = sum >> (BIGUINT_BASE / 2);
		i++;
	}

	while (i < greater_addend->len) {
		uint64_t sum = (uint64_t)greater_addend->value[i] + (uint64_t)carry;
		total_buffer->value[i] = (uint32_t)sum;
		carry = sum >> (BIGUINT_BASE / 2);
		i++;
	}

	if (carry > 0) {
		biguint_increase_len(total_buffer, total_buffer->len * 2);
		total_buffer->value[i] = carry;
	}

}

void biguint_mult(biguint_t* product_buffer, biguint_t* factor1, biguint_t* factor2) {
	for (size_t j = 0; j < factor1->len; j++) {
		for (size_t i = 0; i < factor2->len; i++) {
			// TODO proper multiplication algorithm
			product_buffer->value[i] = factor1->value[i] * factor2->value[i];
		}
	}
}

void biguint_print_base16(FILE* stream, biguint_t* biguint) {
	size_t i = biguint->len;
	fprintf(stream, "0x");

	while (i > 0 && biguint->value[i-1] == 0) {
		i--;
	}

	while (i > 0) {
		fprintf(stream, "%.4x", biguint->value[i-1]);
		i--;
	}
}
