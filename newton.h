#pragma once
/*------------------------------------------------------------*/
/* newton.h                                                   */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/

// Uses Newton's method to produce successively better approximations to
// the roots (or zeroes) of a real-valued function.
double* guess(Polynomial_t poly, double convCrit);
