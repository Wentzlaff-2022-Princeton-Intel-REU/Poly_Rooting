#pragma once

#include "polynomial.h"

/* The evaluate function takes in a polynomial and an input (double), 
then evaluates the polynomial with the given input and returns
the correct value using the Horner method. */
double evaluate(Polynomial_t poly, double x);