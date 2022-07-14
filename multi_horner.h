#pragma once
/*------------------------------------------------------------*/
/* multi_horner.h                                              */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/

/* The multiHorner function takes in a polynomial and an input
(double), then evaluates the polynomial with the given input and
returns the correct value using the Horner method. */
double* multi_horner(Polynomial_t poly, double* guesses);
