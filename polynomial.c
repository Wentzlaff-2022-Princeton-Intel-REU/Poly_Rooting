/*--------------------------------------------------------------------*/
/* polynomial.c                                                          */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "polynomial.h"

/*--------------------------------------------------------------------*/
void freePoly(Polynomial_t* poly) {
    free(poly->coefficients);
}