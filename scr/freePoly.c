/*--------------------------------------------------------------------*/
/* freePoly.c                                                          */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "freePoly.h"

/*--------------------------------------------------------------------*/

void freePoly(Polynomial_t* poly) {
    free(poly->coefficients);
}