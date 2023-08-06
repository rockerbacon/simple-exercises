#pragma once

#include <stdio.h>
#include <stdlib.h>

#define BIGUINT_BASE 64
#define BIGUINT_INIT_LEN 256 / 8 / sizeof(uint32_t)

#include <stdint.h>

typedef struct {
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
 * Increases the length of a biguint_t instance.
 * This is done automatically during arithmetic operations,
 * and explicitly using this function is discouraged.
 *
 * @param biguint the instance to increase
 * @param new_len the new length for the instance. A unit of new_len equals 4 bytes
 */
void biguint_increase_len(biguint_t* biguint, size_t new_len);

/**
 * Sets the value of the biguint_t instance
 *
 * @param biguint instance to modify
 * @param value value to set to the biguint_t instance
 */
void biguint_set(biguint_t* biguint, uint64_t value);

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
 * Converts biguint_t to a decimal string representation,
 * outputing to the provided stream.
 *
 * @param stream location where the string will be written to
 * @param biguint biguint_t to read from
 */
void biguint_print_base10(FILE* stream, biguint_t* biguint);

/**
 * Converts biguint_t to a hexadecimal string representation,
 * outputing to the provided stream.
 *
 * @param stream location where the string will be written to
 * @param biguint value to read from
 */
void biguint_print_base16(FILE* stream, biguint_t* biguint);

