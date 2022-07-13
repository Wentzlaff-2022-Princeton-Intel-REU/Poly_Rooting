#pragma once
/*------------------------------------------------------------*/
/* multiNewton.h                                              */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/

/* Uses Newton's method to produce successively better
approximations to the roots (or zeroes) of a real-valued
function. */
double* multiNewton(Polynomial_t poly, double convCrit);
