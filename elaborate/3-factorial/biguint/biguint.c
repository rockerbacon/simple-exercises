#include <biguint.h>
#include <stdlib.h>

#define DEFAULT_CAPACITY 4096 / 8 / sizeof(uint32_t)

void biguint_alloc_capacity(biguint_t* biguint, size_t capacity) {
	if (biguint->len > capacity) {
		biguint->len = capacity;
	}

	biguint->capacity = capacity;
	biguint->value = (uint32_t*)realloc(biguint->value, capacity*sizeof(uint32_t));
}

void biguint_init(biguint_t* biguint) {
	biguint->len = 0;
	biguint->capacity = 0;
	biguint->value = NULL;
	biguint_alloc_capacity(biguint, DEFAULT_CAPACITY);
}

void biguint_destroy(biguint_t* biguint) {
	free(biguint->value);
}

void biguint_set(biguint_t* biguint, uint32_t value) {
	biguint->value[0] = value;
	biguint->len = 1;
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

	if (total_buffer->capacity < longer_addend->len + 1) {
		biguint_alloc_capacity(total_buffer, total_buffer->capacity * 2);
	}

	size_t i = 0;
	uint32_t carry = 0;
	while (i < shorter_addend->len) {
		uint64_t sum =
			(uint64_t)shorter_addend->value[i] +
			(uint64_t)longer_addend->value[i] +
			(uint64_t)carry;

		total_buffer->value[i] = (uint32_t)sum;
		carry = sum >> 32;
		i++;
	}

	while (i < longer_addend->len) {
		uint64_t sum = (uint64_t)longer_addend->value[i] + (uint64_t)carry;
		total_buffer->value[i] = (uint32_t)sum;
		carry = sum >> 32;
		i++;
	}

	if (carry > 0) {
		total_buffer->value[i] = carry;
		total_buffer->len = i+1;
	} else {
		total_buffer->len = i;
	}
}

void biguint_mult(biguint_t* product_buffer, biguint_t* factor1, biguint_t* factor2) {
	if (product_buffer->len < factor1->len + factor2->len) {
		biguint_alloc_capacity(product_buffer, (factor1->len + factor2->len) * 2);
	}

	for (size_t i = 0; i < factor1->len + factor2->len; i++) {
		product_buffer->value[i] = 0;
	}

	uint32_t carry = 0;
	for (size_t i = 0; i < factor1->len; i++) {
		carry = 0;

		for (size_t j = 0; j < factor2->len; j++) {
			uint64_t summed_product =
				(uint64_t)factor1->value[i] *
				(uint64_t)factor2->value[j] +
				(uint64_t)product_buffer->value[i+j] +
				(uint64_t)carry;

			product_buffer->value[i+j] = (uint32_t)summed_product;
			carry = summed_product >> 32;
		}

		product_buffer->value[factor2->len + i] = carry;
	}

	if (carry > 0) {
		product_buffer->len = factor1->len + factor2->len;
	} else {
		product_buffer->len = factor1->len + factor2->len - 1;
	}
}

void biguint_print_base16(FILE* stream, biguint_t* biguint) {
	size_t i = biguint->len;
	fprintf(stream, "0x");

	while (i > 0) {
		fprintf(stream, "%.8x", biguint->value[i-1]);
		i--;
	}
}
