#pragma once
/*------------------------------------------------------------*/
/* horner.h                                                   */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/

/* The evaluate function takes in a polynomial and an input (double), 
then evaluates the polynomial with the given input and returns
the correct value using the Horner method. */
vfloat64m1_t horner(Polynomial_t poly, vfloat64m1_t vGuesses, size_t guessSize);
