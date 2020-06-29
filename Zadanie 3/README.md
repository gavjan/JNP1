# Zeckendorf Representation in C++

## The Representation
 
Zeckendorf Representation or Fibonacci system is a way to represent integers using sum of consecutive Fibonacci numbers.
Similar to binary representation, except instead of consecutive powers of two, Fibonacci numbers are used.

A digit from the Fibonacci system is called fibit.
A number in Fivonacci system is in standardized if among its digits there are no two 1s next to each other(two fibits equal to one).

More about Zeckendorf Representation:
https://en.wikipedia.org/wiki/Zeckendorf%27s_theorem

## Implementation
In the provided implementation Zeckendorf numbers are stored as boolean vectors since `std::vector<bool>` is space optimized to use 1-bit `bool` values instead of 1-byte

Below are the supported functions for the class `Fibo`:

- `Fibo f1` - creates the number 0
- `Fibo f1(str)` - creates a number based on the string `str`, which is a record of this numbers in the Fibonacci system (not necessarily must be in standardized form)
- `Fib(n)` - creates a Zeckendorf number equal to the integer n
- `Fibo f1(f2)` - copy constructor
- `f1 op f2` - where `op` is one of the following operations: 
    - `f1 = f2` - assignment
    - `f1 + f2` - addition
    - `f1 & f2` - AND on fibits(in normalized form)
    - `f1 | f2` - OR on fibits(in normalized form)
    - `f1 ^ f2` - XOR on fibits(in normalized form)
    - `f1 << n` - fibit shift to the left by n positions

    their respective operations with assigment:
    - `f1 += f2` 
    - `f1 &= f2`
    - `f1 |= f2`
    - `f1 ^= f2`
    - `f1 <<= n`


- `f1 op f2` - operators comparing numerical values represented by `f1` and `f2`, where op is one of the operators: `==`, `<`, `<=`,`!` `=`,`>`,`>` `=`
- `os << f1` - prints `f1` to the stream os (in normalized form) without any whitespace characters
- `f1.length()` - returns the length of the normalized Zeckendorf number `f1`
  