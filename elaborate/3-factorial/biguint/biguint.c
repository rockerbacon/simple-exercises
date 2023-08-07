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
	biguint_t* longer_addend;
	biguint_t* shorter_addend;

	if (addend1->len < addend2->len) {
		longer_addend = addend2;
		shorter_addend = addend1;
	} else {
		longer_addend = addend1;
		shorter_addend = addend2;
	}

	if (total_buffer->len < longer_addend->len) {
		biguint_increase_len(total_buffer, longer_addend->len + 1);
	}

	size_t i = 0;
	uint32_t carry = 0;
	while (i < shorter_addend->len) {
		uint64_t sum =
			(uint64_t)shorter_addend->value[i] +
			(uint64_t)longer_addend->value[i] +
			(uint64_t)carry;

		total_buffer->value[i] = (uint32_t)sum;
		carry = sum >> (BIGUINT_BASE / 2);
		i++;
	}

	while (i < longer_addend->len) {
		uint64_t sum = (uint64_t)longer_addend->value[i] + (uint64_t)carry;
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
	biguint_set(product_buffer, 0);
	if (product_buffer->len < factor1->len + factor2->len + 1) {
		biguint_increase_len(product_buffer, (factor1->len + factor2->len) * 2);
	}

	for (size_t i = 0; i < factor1->len; i++) {
		uint32_t carry = 0;
		for (size_t j = 0; j < factor2->len; j++) {
			uint64_t summed_product =
				(uint64_t)factor1->value[i] *
				(uint64_t)factor2->value[j] +
				(uint64_t)product_buffer->value[i+j] +
				(uint64_t)carry;

			product_buffer->value[i+j] = (uint32_t)summed_product;
			carry = summed_product >> (BIGUINT_BASE / 2);
		}

		product_buffer->value[factor2->len + i] = carry;
	}
}

void biguint_print_base16(FILE* stream, biguint_t* biguint) {
	size_t i = biguint->len;
	fprintf(stream, "0x");

	while (i > 0 && biguint->value[i-1] == 0) {
		i--;
	}

	while (i > 0) {
		fprintf(stream, "%.8x", biguint->value[i-1]);
		i--;
	}
}
