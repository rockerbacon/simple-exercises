package bigmath

import (
	"math/big"
)

var constant struct {
	one big.Int;
	two big.Int;
}

func init() {
	constant.one.SetUint64(1)
	constant.two.SetUint64(2)
}

func factorial(
	number *big.Int,
	decrementBuffer *big.Int,
	accumulator *big.Int,
	productBuffer *big.Int,
) *big.Int {
	if (number.Cmp(&constant.two) < 0) {
		return accumulator
	}

	productBuffer.Mul(accumulator, number)
	decrementBuffer.Sub(number, &constant.one)

	return factorial(decrementBuffer, number, productBuffer, accumulator)
}

func Factorial(number uint32) *big.Int {
	if (number == 0) {
		return big.NewInt(1)
	}

	accumulator := big.NewInt(int64(number));
	nextNumber := big.NewInt(int64(number - 1));
	decrementBuffer := big.NewInt(0);
	productBuffer := big.NewInt(0);

	return factorial(nextNumber, decrementBuffer, accumulator, productBuffer)
}

func BadFactorial(number uint32) *big.Int {
	if (number < 2) {
		return big.NewInt(1)
	}

	bignumber := big.NewInt(int64(number))
	resultBuffer := big.NewInt(0)

	return resultBuffer.Mul(bignumber, BadFactorial(number - 1))
}

