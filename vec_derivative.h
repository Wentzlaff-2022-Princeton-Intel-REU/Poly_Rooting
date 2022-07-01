#pragma once
/*------------------------------------------------------------*/
/* vec_derivative.h                                               */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/

// The differentiatePoly function takes in a Polynomial_t object as input,
// differentiates it, and returns a differentiated Polynomial_t object 
// as output. 
Polynomial_t vec_differentiatePoly(Polynomial_t myPoly);

