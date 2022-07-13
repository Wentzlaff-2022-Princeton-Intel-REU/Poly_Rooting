#pragma once
/*------------------------------------------------------------*/
/* longDiv.h                                                   */
/*------------------------------------------------------------*/

#include "polynomial.h"

/*------------------------------------------------------------*/

/* performs long division on a polynomial dividend and a linear
polynomial divisor and returns a polynomial quotient */
Polynomial_t longDiv(Polynomial_t poly, double root, double diff);