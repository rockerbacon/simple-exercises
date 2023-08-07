#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	size_t capacity;
	size_t len;
	uint32_t *value;
} biguint_t;

typedef char* biguint_str;

/**
 * Initializes a new biguint_t instance.
 * All variables of type biguint_t must be initialized exactly once before usage.
 *
 * @param biguint variable to initialize
 */
void biguint_init(biguint_t* biguint);

/**
 * Destroys a biguint_t instance.
 * All variables of type biguint_t must be destroyed exactly once, at the end of their lifetime.
 * biguint_t variables cannot be used after destruction.
 *
 * @param biguint instance to destroy
 */
void biguint_destroy(biguint_t* biguint);

/**
 * Sets the capacity of a biguint_t instance.
 * This is done automatically during initialization and arithmetic operations,
 * and explicitly using this function is discouraged.
 *
 * @param biguint the instance to set
 * @param capacity the new capacity for the instance. Value must be in digits, and the base is 2^32
 */
void biguint_alloc_capacity(biguint_t* biguint, size_t capacity);

/**
 * Sets the value of the biguint_t instance
 *
 * @param biguint instance to modify
 * @param value value to set to the biguint_t instance
 */
void biguint_set(biguint_t* biguint, uint32_t value);

/**
 * Multiplies two biguint_t instances
 *
 * @param product_buffer buffer where the result of the operation will be stored
 * @param factor1 first factor of the multiplication
 * @param factor2 second factor of the multiplication
 */
void biguint_mult(biguint_t* product_buffer, biguint_t* factor1, biguint_t* factor2);

/**
 * Sums two biguint_t instances
 *
 * @param total_buffer buffer where the result of the operation will be stored
 * @param addend1 first addend of the sum
 * @param addend2 second addend of the sum
 */
void biguint_sum(biguint_t* total_buffer, biguint_t* addend1, biguint_t* addend2);

/**
 * Calculates the factorial of a number, utilizing biguint arithmetic
 *
 * @param result_buffer buffer where the result of the operation will be stored
 * @param number the number for which to calculate the factorial
 */
void biguint_factorial(biguint_t* result_buffer, uint32_t number);

/**
 * Converts biguint_t to a hexadecimal string representation,
 * outputing to the provided stream.
 *
 * @param stream location where the string will be written to
 * @param biguint value to read from
 */
void biguint_print_base16(FILE* stream, biguint_t* biguint);

