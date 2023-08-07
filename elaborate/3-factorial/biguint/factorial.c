#include<biguint.h>

#define swap_ptr(a, b) {\
	void* tmp = a;\
	a = b;\
	b = tmp;\
}

void biguint_factorial(biguint_t* result_buffer, uint32_t number) {
	biguint_t additional_buffer;
	biguint_t scalar_buffer;

	biguint_t* factor_buffer;
	biguint_t* product_buffer;

	if (number < 3) {
		biguint_set(
			result_buffer,
			number == 0 ? 1 : number
		);
		return;
	}

	factor_buffer = &additional_buffer;
	product_buffer = result_buffer;

	biguint_init(&scalar_buffer);
	biguint_init(&additional_buffer);

	biguint_set(factor_buffer, 2);
	for (uint32_t scalar = 3; scalar <= number; scalar++) {
		biguint_set(&scalar_buffer, scalar);
		biguint_mult(product_buffer, factor_buffer, &scalar_buffer);
		swap_ptr(product_buffer, factor_buffer);
	}

	if (factor_buffer != result_buffer) {
		swap_ptr(additional_buffer.value, result_buffer->value);
		result_buffer->len = additional_buffer.len;
		result_buffer->capacity = additional_buffer.capacity;
	}

	biguint_destroy(&scalar_buffer);
	biguint_destroy(&additional_buffer);
}

