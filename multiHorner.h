#pragma once
/*------------------------------------------------------------*/
/* multiHorner.h                                              */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/

/* The multiHorner function takes in a polynomial and an input
(double), then evaluates the polynomial with the given input and
returns the correct value using the Horner method. */
double* multiHorner(Polynomial_t poly, double* guesses);
