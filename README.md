<h1 align="center">BigReal Class</h1>
<!---->
<!-- ### Features -->
<!---->
<!-- - Addition and subtraction of `BigReal` numbers. -->
<!-- - Comparison operators (`<`, `>`, `==`) for `BigReal` instances. -->
<!-- - Input and output operators (`>>` and `<<`) for convenient interaction. -->

## Table of Contents
- [Introduction](#introduction)
- [Class Overview](#class-overview)
- [Constructor](#constructor)
- [Assignment Operators](#assignment-operators)
- [Arithmetic Operators](#arithmetic-operators)
- [Relational Operators](#relational-operators)
- [IO Operators](#io-operators)
- [Getter Methods](#getter-methods)
- [Example Usage](#example-usage)

## Introduction
The `BigReal` class is designed to represent real numbers with both integer and fractional components. It can handle large numbers that exceed the limitations of standard C++ data types by utilizing the `BigDecimalInt` class for managing integer digits.

## Class Overview
The class has the following private members:
- `BigDecimalInt integerDigits`: Represents the integer part of the real number.
- `BigDecimalInt fractionDigits`: Represents the fractional part of the real number.
- `char Sign`: Represents the sign of the real number ('+', '-' ).

It also provides various member functions for performing arithmetic operations, handling carries and borrows, and managing the format of fractions.

## Constructor
The `BigReal` class provides several constructors:
- `BigReal(double = 0.0)`: Default constructor.
- `BigReal(string)`: Constructor to initialize from a string.
- `BigReal(BigDecimalInt)`: Constructor to initialize from a `BigDecimalInt` object.
- `BigReal(BigDecimalInt, BigDecimalInt, char)`: Constructor to initialize all components explicitly.

## Assignment Operators
The class supports assignment from various types, including `string`, `BigDecimalInt`, `int`, and `long long`.

## Arithmetic Operators
The class supports addition and subtraction operations for `BigReal` objects.

## Relational Operators
Relational operators such as `<`, `>`, and `==` are implemented to compare `BigReal` objects.

## IO Operators
Input and output stream operators (`<<` and `>>`) are overloaded to facilitate easy input and output of `BigReal` objects.

## Getter Methods
The class provides methods to retrieve the size and sign of the `BigReal` object.

## Example Usage
Here is an example of how to use the `BigReal` class:

```cpp
#include "BigReal.h"
#include <iostream>
// Your code goes here
```
---
### Authors
* [Mina Albert](https://github.com/minaalbert33)
* [Ahmed Zaher](https://github.com/AhmedMZaher)


