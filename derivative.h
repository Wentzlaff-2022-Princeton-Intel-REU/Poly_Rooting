#pragma once
/*--------------------------------------------------------------------*/
/* derivative.h                                                       */
/* Author: Mukund Ramakrishnan                                        */
/*--------------------------------------------------------------------*/

#include "polynomial.h"

// The differentiatePoly function takes in a Polynomial_t object as input,
// differentiates it, and returns a differentiated Polynomial_t object 
// as output. 
Polynomial_t differentiatePoly(Polynomial_t myPoly);

