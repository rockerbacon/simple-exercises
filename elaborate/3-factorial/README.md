## Introduction

Here you will find an in-depth analysis of the challenges in computing factorials,
as well as practical solutions to these challenges.

First we will define what factorials are and a simple algorithm for computing it.
Then we will go over some of the limitations of this simple solution and explore
options to overcome these limitations.

Source code for the solutions documented here are available under the `c` and `go` directories.
The `python` directory contains a program which uses Python's built-in factorial implementation
as a point of reference. We will go over the available code in more details in the subsequent sections.

## Defining Factorial

The factorial of a number _n_ is denoted by _n!_ and is equal to the product of all positive integers in the interval [1, n]:
```
n! = n * (n-1) * (n-2) * ... * 3 * 2 * 1
```

0! can also be defined through the definition of a vacuous product: multiplying no factors equals the multiplicative identity (1).

One simple and clear way to define factorial is using the simple recurrence relation:
```
n! = n * (n-1)! for n > 0,
0! = 1
```

## The Simplest Solution

The (arguably) simplest algorithm to calculate factorials is to simply write its recurrence relation as code:
```
fn factorial(n)
    if n = 0 then
        return 1
    endif

    return n * factorial(n-1)
endfn
```

This, in fact, was the chosen solution for the minimalistic code contained in this repository.

There are, however, severe issues when deploying this algorithm into real machines.
We will go over these issues in the sections below.

### Growth

Creating computer programs for calculating very small factorials is very simple,
the main difficulty arises when attempting to calculate it for larger numbers.
Factorial has faster than exponential growth,
meaning it is impossible to calculate it using just hardware multiplication instructions.

21! is the largest possible value that can be calculated when using 64 bit unsigned integers.
The value is so small that, when restricted to 64 bit unsigned integers,
the best algorithm is to simply pre-calculate all values and return them, for a _O(1)_ solution:

```
factorial_result = [
    0x1,
    0x1,
    0x2,
    0x6,
    ...,
    0x2c5077d36b8c40000
]

fn factorial(n)
    if n > 21 then
        error "factorial(n) would overflow a 64 bit unsigned integer"
    endif

    return factorial_result[n]
endfn
```

64 bit floating point numbers can hold larger values before overflowing (up to 170!),
but they have unreliable integer precision for values larger than 2^53 + 1 (maximum of 19!).

Computing larger factorials requires utilizing arbitrary-precision arithmetic.
We will detail how this can be achieved in the **a better solution** section below.

### Stack Limits

Computer programs have limited stack space. The exact limit is primarily determined by the operating system,
altough some programming languages and execution environments often enforce stack depth limits of their own.

A recursive factorial implementation is susceptible to reaching stack limits.
Since the program has to maintain a stack for each recursive call, the algorithm ends up having _O(n)_ space complexity,
which becomes a problem for the often very limited stack resources.

We will explore means of mitigating this drawback of recursive functions in the **a better solution** section below.

## A Better Solution

In this section, we will first talk about the theory necessary for solving the problems described
in the previous section.
Then we will go through the available factorial implementations and talk about how they
apply the theory in order to create a practical solution.

At the end of the section,
we will look at some benchmarks to see how the solutions perform in comparison to one another.

### Solving the Growth Problem

Arbitrary-precision arithmetic must be used in order to solve the growth problem when calculating factorials larger than 21!.

It consists in representing numbers using dynamically-sized vectors, where each item in the vector represents a digit.
With this representation, it is then necessary to program fundamental operations (sum, multiplication, etc).

Many languages offer built-in functionality for arbitrary-precision arithmetic,
that is commonly found under names such as _BigInt_ (JavaScript, Go) or _BigInteger_ (Java, C#).
In fact, the minimalistic code in this repository isn't even affected by the growth problem because Python
defaults to arbitrary-precision arithmetic for all integers (starting from version 3.0+).

The usage of arbitrary-precision arithmetic severely impacts the performance of factorial implementations.

Following the standard factorial definition, any program will have to do at least _n_ multiplications.
Fixed-precision arithmetic (bult-in hardware instructions) can perform a multiplication with a single operation,
leaving us with an asymptotic complexity of _O(n * 1)_ or just _O(n)_. Arbitrary-precision arithmetic, however,
requires various operations for performing a single multiplication.

There are various algorithms for performing arbitrary-precision multiplications.
The most well-known one is _long multiplication_, which multiplies two values in _O(x*y)_, where _x_ and _y_ are the number of digits of each factor
(this is the algorithm thought to children at schools).
More advanced algorithms can perform better, with the best known algorithm having _O(k*log(k))_ complexity,
where _k_ is the maximum number of digits of both factors.

In order to roughly estimate the real complexity of our factorial algorithm,
we want to know how many digits there are in _n_ and _(n-1)!_, since the last multiplication (_n * (n-1)!_) is the most costly.
We can define the number of digits in _n_ simply as `d(n)=floor(log(n)) + 1`.
For _(n-1)!_, it is possible to use Stirling's formula to approximately estimate the number of digits the result of a given factorial will have,
which will lead us to `d((n-1)!) = (n-1)log(n-1) = n*log(n-1) - log(n-1)`.

By multiplying our _O(n)_ factorial complexity with our _O(x*y)_ multiplication complexity, we can calculate our real-world factorial complexity:
```
O(n*x*y) =
O(n * d((n-1)!) * d(n)) =
O(n * [n*log(n-1) - log(n-1)] * [floor(log(n) + 1)])

Now we use the following assymptotic simplifications:
1. O(floor(log(n) + 1)) is contained in O(log(n));
2. O(n*log(n-1) - log(n-1)) is contained in O(n*log(n));

O(n * [n*log(n-1) - log(n-1)] * [floor(log(n) + 1)]) =
O(n * n * log(n) * log(n)) =

O(n^2 * log^2(n))
```

### Avoiding Stack Limits

The easiest way to avoid stack limits is to just increase them.
When using a programming language with no additional limits,
simply increasing the OS's program stack size is enough.
For languages with their own limits,
there are usually means to modify the limits.

Limits can be avoided altogether by not using additional stack.
This can be achieved by either converting a recursive algorithm
into an iterative one - which is always possible, as the famous Church-Turing thesis ensures us -
or by leveraging tail call optimization features.

Tail call optimization can be done whenever the last action a function performs is a call to another one.
In this situation, there is no need to retain the state of the caller function and the compiler/interpreter
can simply replace the current stack instead of creating another one.

The following is an example of a recursive factorial function which can be tail call optimized.
Note that the return statement includes only a function call and nothing else.
```
fn tail_call_factorial(n, accumulator = 1)
    if n = 0 then
        return accumultor
    endif

    return tail_call_factorial(n-1, n * accumulator)
endfn
```

### C Implementation

For learning purposes, the C code includes an arbitrary-precision arithmetic implementation written from scratch.

The code represents unsigned integers, using a vector of 32 bit unsigned integers,
where each item represents a base 2^32 digit.
Such a large numeric base allows us to reduce the number of operations required,
since the number of digits a value has is inversely proportional to the numeric base.
The representation is little endian,
meaning that the least significant digit is at the index 0 of the vector.

The code provides functions for summing, multiplying and printing values in a hexadecimal base.
_long multiplication_ was the multiplication algorithm of choice.
Numbers are printed in hexadecimal instead of decimal simply because the standard C formatting
functionality is already capable of converting any 2^x base to hexadecimal.

The arbitrary-precision source code can be found at `elaborate/3-factorial/c/biguint/biguint.c`.

In order to avoid stack limits, an iterative factorial algorithm is used.
The factorial source code is available at `elaborate/3-factorail/c/biguint/naive_factorial.c`.

### Go Implementation

We will use Go to create a real world example on how to leverage tail call optimizations.
The source code is available at `elaborate/3-factorial/go/bigmath/factorial.go`

We will implement both the tail call factorial algorithm (function _Factorial_), described in section **Avoiding Stack Limits**,
as well as the simple algorithm from section **The Simplest Solution** (function _BadFactorial_).

One can switch between implementations by editing the main program at `elaborate/3-factorial/go/main.go`.
In the test system used, _BadFactorial_ can only calculate up to 5,592,385!,
while _Factorial_ should only be limited by available system memory and was able to calculate 1,000,000,000! during testing.

#### Additional Memory Management Optimizations

The tail call factorial algorithm also includes memory allocation optimizations.
Altough it may be difficult to understand at first, the process is rather simple.
It works by allocating a front and back buffer for each mathematical operation
(in our case the subtraction _n-1_ and the multiplication _n * accumulator_)
and swapping around the front and back buffers while running the recursive call.
This methods allows buffers to be allocated only once, when the program initializes,
instead of a new allocation being required for each operation.

The idea can be better visualized with these simpler examples:
```
fn recursive_summation(iterator, front_accumulator, back_accumulator):
    if iterator.ended() then
        return front_accumulator
    endif

    back_accumulator.store(front_accumulator + iterator.get_item())

    return recursive_summation(iterator.next(), back_accumulator, front_accumulator)
endfn

fn start_recursive_summation(vector):
    allocate front_accumulator
    allocate back_accumulator

    iterator = vector.begin()
    front_accumulator.store(iterator.get_item())

    return recursive_summation(iterator.next(), front_accumulator, back_accumulator)
endfn

fn iterative_summation(vector)
    allocate front_accumulator
    allocate back_acculumator

    front_accumulator.store(vector[0])

    for i from 1 to vector.length() do
        item = vector[i]
        back_accumulator.store(front_accumulator + item)
        swap(back_accumulator, front_accumulator)
    endfor

    return front_accumulator
endfn
```

This is not a particularly useful optimization for our use case because our memory usage is very low.
It is extremely useful, tought, in applications where memory requirements are high relative to
available memory resources.
Since Go uses a garbage collector, it is not possible to control when unused buffers will be freed from memory -
this can exhaust memory resources if too many relatively large allocations are executed in a short time-span.
Reducing the amount of variables in the heap is also benefitial to garbage collected languages in general,
as it tends to reduce garbage collector overhead which, in turn, reduces intermittent performance problems.

### Python Implementation

The Python implementation will serve merely as a point of reference.
We will use the bult-in `math.factorial` as a reference point, both in execution time and output,
for our own implementations.

### Bechmarks

The repository includes a simple script to execute the banchmarks, available at `elaborate/3-factorial/benchmark.py`.
It is necessary to build the C and Go implementations before running the benchmarks.

The benchmarks were run on a Ryzen 4300U processor and these were results:

|              |    Python   |     Go      |      C      |
|    :---:     |    :---:    |    :---:    |    :---:    |
|  10!         |  00:00.055  |  <00:00.001 |  00:00.001  |
|  100!        |  00:00.032  |  <00:00.001 |  00:00.001  |
|  1,000!      |  00:00.050  |  <00:00.001 |  00:00.001  |
|  10,000!     |  00:00.041  |  00:00.012  |  00:00.052  |
|  100,000!    |  00:00.125  |  00:02.763  |  00:06.325  |
|  1,000,000!  |  00:06.638  |  06:27.008  |  14:36:694  |

## An Even Better Solution

We can see from the benchmark results in the previous section that Python's built-in factorial function greatly
outperforms both of our implementations when the numbers get sufficiently large but it can't compete when it comes to small numbers.
This is a pretty simple case of algorithmic optimization vs hardware optimization.

### Small Numbers Are Easy

Small numbers can be computed extremely fast, even for our _O(n^2 * log^2(n))_ algorithm.
In this situation, the lack of an interpreter makes C and Go faster simply because they have near zero overhead.
Python's 41-55ms execution times are likely to be just the cost of initializing the interpreter and reading the code.

### Large Numbers Are Very Hard

The truth for the factorial operation is that just multiplying the numbers in sequence is extremely inneficient.
Because the number of digits grows extremely fast, multiplications get increasing costly as the value to calculate grows.
Remember: the complexity of factorial itself is just _O(n)_, it's the arbitrary-precision multiplication that
brings it to _O(n^2*log^2(n))_.

### Improving Even Further

Taking the previous complexity analysis into consideration, there are a few improvement paths we can take.

#### Not using the factorial definition at all

Instead of multiplying all numbers in the interval [1, n],
one can instead find an algorithm that computes factorial without utilizing its definition.

Arnold Schonange has designed one such algorithm, which can compute factorial in _O(n*log^2(n))_ using
prime factorization and exponentiation by squaring.

Altough this is by far be the best possible solution, it is too complex for the time constraints of this small project.

#### Using a faster multiplication algorithm

As described in previous sections, faster multiplication algorithms do exist.

Algorithms can also be designed to better utilize hardware features and, in real-world scenarios,
two algorithms with similar complexity can have vastly different performance.

This is likely to be the main reason why the Go implementation is far superior to the C implementation:
in Go, we used the built-in _big.Int_ type, which is likely to have a lot better multiplication implementation
than our simple long multiplication algorithm that was quickly written in less than a day.

#### Arranging operations to reduce the number of digits

Let's take a look at the number of digits _k_ in a factorial result, analyze how it grows,
and see if we can do something to slow down this growth.

Our long multiplication algorithm will perform _O(x*y)_ operations, where _x_ and _y_ are the number of digits in 
the factors.

From there, let's look at this simple example where we calculate 8!:

| step |   factor1   |   factor2    |  result  | operations required |
| ---: |    :---:    |    :---:     |   :---:  |       :---          |
| 1    |     8       |      7       |    56    |        1            |
| 2    |     56      |      6       |    336   |        2            |
| 3    |     336     |      5       |   1680   |        3            |
| 4    |    1680     |      4       |   6720   |        4            |
| 5    |    6720     |      3       |   20160  |        4            |
| 6    |   20160     |      2       |   40320  |        5            |

In total, we require 19 operations to process 8! when multiplying in this order.
Let's see what happends now if we divide the multiplications in two groups and combine the two,
that is, let's calculate (8 * 7 * 6) * (5 * 4 * 2)

| step |   factor1   |   factor2    |  result  | operations required |
| ---: |    :---:    |    :---:     |   :---:  |       :---          |
| 1    |     8       |      7       |    56    |        1            |
| 2    |     56      |      6       |    336   |        2            |
| 3    |     336     |      5       |   1680   |        3            |
| 4    |     4       |      3       |    12    |        1            |
| 5    |     12      |      2       |    24    |        2            |
| 6    |    1680     |      24      |   40320  |        8            |

You will see that only 17 operations are required when reordering the operations like that.
This happens because the number of multiplications for both processes stays the same,
but the rearrangement leads to them having smaller factors,
which reduces the number of operations performed by the arbitrary-precision multiplication.

Now we can apply this concept recursively to achieve a _O(n*log^3(n))_ factorial algorithm:
```
fn multiply_range(start, end)
    if start = end then
        return start
    else if start > end then
        return 1 
    endif

    middle = (start + end) / 2
    left_result = multiply_range(start, middle)
    right_result = multiply_range(middle + 1, end)

    return left_result * right_result
endfn

fn factorial(n)
    if n = 0 then 
        return 1
    else if n < 3 then
        return n
    endif

    return multiply_range(2, n)
endfn
```

We will skip the asymptotic analysis for this one.

### New Benchmarks

Now let's implement the more efficient divide and conquer algorithm in Go and C and benchmark again.
The source code is available at `elaborate/3-factorial/go/bigmath/factorial.go, func FastFactorial`
and `elaborate/3-factorial/c/biguint/factorial.c`.

|              |    Python   |     Go      |      C      |
|    :---:     |    :---:    |    :---:    |    :---:    |
|  10!         |  00:00.055  |  00:00.001  | <00:00.001  |
|  100!        |  00:00.032  |  00:00.001  | <00:00.001  |
|  1,000!      |  00:00.050  |  00:00.001  | <00:00.001  |
|  10,000!     |  00:00.041  |  00:00.004  |  00:00.014  |
|  100,000!    |  00:00.125  |  00:00.084  |  00:00.883  |
|  1,000,000!  |  00:06.638  |  00:02.813  |  02:22.663  |

Now the Go implementation managed to beat the Python built-in in all scenarios.

The C implementation is still quite slow in comparison.
That is likely due to our simplistic, quickly written long multiplication code.

