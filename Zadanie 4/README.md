# Fibin
Metaprogramming using C++ templates is Turing Complete.
To make sure in that, we will try to create a simple functional language and its interpreter based on C++ templates.

The Fibin language supports:

##### Literals Lit
Numeric literals can only be Fibonacci numbers, e.g. Fib <0> = 0, Fib <1> = 1 etc.
Fib parameters must be non-negative.
Logical literals can only be two logical values: True, False.
Examples of valid literals: Lit <Fib <0>>, Lit <True>.

##### Var variables
Var (const char *) - variable identifier; identifiers are created based on
strings 1 to 6 inclusive and contain lowercase and uppercase letters
English alphabet (a-zA-Z) and numbers (0-9); Lowercase and uppercase letters are not
distinguished.
Examples of valid variables: Var ("A"), Var ("01234"), Var ("Cdefg").

##### Arithmetic operations Sum, Inc1, Inc10
Sum <...> - operation of adding many numbers, at least two arguments are required.
Inc1 <Arg> - addition specialization that increases the Arg value by Fib <1>.
Inc10 <Arg> - addition specialization that increases the Arg value by Fib <10>.
Examples of correct operations: Sum <Lit <Fib <0>>, Lit <Fib <1>>, Lit <Fib <3> >>,
INC1 <Lithium <Fib <0 >>>.

##### Eq comparison
Eq <Left, Right> - compares the value of Left with Right; returns True when they are equal,
and False otherwise.
An example of a correct comparison: Eq <Lit <Fib <0>>, Lit <Fib <1> >>.

##### Reference to variable Ref
Ref <Var> - returns the value of the variable identified by Var.
An example of a valid reference to a variable: Ref <Var ("A")>.

##### Let expression
Let <Var, Value, Expression> - assigns Value to the variable Var
and calculates the value of Expression.
An example of a valid expression: Let <Var ("A"), Lit <Fib <1>>, Ref <Var ("A") >>.

##### If statement
If <Condition, Then, Else> - if Condition is True, then calculates the value Then,
otherwise it calculates the Else value.
An example of a valid expression: If <Lit <True>, Lit <Fib <1>>, Lit <Fib <0> >>.

##### Lambda expression
Lambda <Var, Body> - represents an anonymous function with one Var parameter
and Body Body.
An example of a valid expression: Lambda <Var ("x"), Ref <Var ("x") >>.

##### Function call
Invoke <Fun, Param> - calculates the value of the function Fun for the parameter Param.
An example of a valid expression: Invoke <Lambda <Var ("x"), Ref <Var ("x") >>, Lit <Fib <0> >>.
The Fibin template class must be provided:

##### Fibin \<ValueType>
ValueType is the type of value used to calculate the value expressions.
The Fibin class provides a public template method:

##### ValueType eval \<Expr>
this method is available only if ValueType is a numeric type.
Calculates the value of Expr during compilation and returns this value.

##### void eval \<Expr> 
this method is available only when ValueType is NOT a numeric type.
Displays information indicating that Fibin does not support that type.

An example of use is given in the file fibin_example.cc. The result of the example is:
```
Fibin doesn't support: PKc
Fibin works fine!
```

## Functional Requirements
This task requires the use of the Clang compiler, which prints very detailed error messages when using templates and metaprogramming. The solution will be compiled using the command
```shell script
clang -Wall -Wextra -std = c ++ 17 -O2 -lstdc ++
```
