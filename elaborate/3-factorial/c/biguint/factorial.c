#include<biguint.h>

#define swap_ptr(a, b) {\
	void* tmp = a;\
	a = b;\
	b = tmp;\
}

void ranged_factorial(biguint_t* result_buffer, size_t range_start, size_t range_end) {
	if (range_start == range_end) {
		biguint_set(result_buffer, range_start);
		return;
	} else if (range_start > range_end) {
		biguint_set(result_buffer, 1);
		return;
	}

	biguint_t left_buffer;
	biguint_t right_buffer;

	biguint_init(&left_buffer);
	biguint_init(&right_buffer);

	size_t middle = (range_start + range_end) / 2;
	ranged_factorial(&left_buffer, range_start, middle);
	ranged_factorial(&right_buffer, middle + 1, range_end);

	biguint_mult(result_buffer, &left_buffer, &right_buffer);

	biguint_destroy(&left_buffer);
	biguint_destroy(&right_buffer);
}

void biguint_factorial(biguint_t* result_buffer, uint32_t number) {
	if (number < 3) {
		biguint_set(
			result_buffer,
			number == 0 ? 1 : number
		);
		return;
	}

	ranged_factorial(result_buffer, 2, number);
}

