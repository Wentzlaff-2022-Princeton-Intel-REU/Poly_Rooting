#pragma once
/*------------------------------------------------------------*/
/* multi_newton.h                                              */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/

/* Uses Newton's method to produce successively better
approximations to the roots (or zeroes) of a real-valued
function. Returns an array of all the real roots of the given
polynomial. */
double* multi_newton(Polynomial_t poly, double convCrit);