/*--------------------------------------------------------------------*/
/* derivative.c                                                       */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include "derivative.h"

/*--------------------------------------------------------------------*/

Polynomial_t derivative(Polynomial_t myPoly) {

    Polynomial_t differentiatedPoly;

    differentiatedPoly.coefficients [myPoly.degree];   
    differentiatedPoly.degree = myPoly.degree - 1;

    for (int counter = myPoly.degree; counter > 0; counter--){
        differentiatedPoly.coefficients[counter - 1] = myPoly.coefficients[counter] * counter;
    }

    return differentiatedPoly;
}
