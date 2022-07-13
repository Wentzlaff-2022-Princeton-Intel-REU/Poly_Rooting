/*--------------------------------------------------------------------*/
/* derivative.c                                                       */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include "derivative.h"

/*--------------------------------------------------------------------*/

Polynomial_t differentiatePoly(Polynomial_t myPoly) {

    Polynomial_t differentiatedPoly;

    differentiatedPoly.coefficients = (double*)malloc(sizeof(double) * (myPoly.degree + 1 - 1));   
    differentiatedPoly.degree = myPoly.degree - 1;

    for (int counter = myPoly.degree; counter > 0; counter--){
        differentiatedPoly.coefficients[counter - 1] = myPoly.coefficients[counter] * counter;
    }

    return differentiatedPoly;
}
