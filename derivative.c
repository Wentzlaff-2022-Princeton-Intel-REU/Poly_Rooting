/*--------------------------------------------------------------------*/
/* derivative.c                                                       */
/* Author: Mukund Ramakrishnan                                        */
/*--------------------------------------------------------------------*/

#include "derivative.h"

/*--------------------------------------------------------------------*/



// This is a root-finding program that will take the derivative of a polynomial.
// It depends on what the structure is



Polynomial_t differentiatePoly(Polynomial_t myPoly) {




    double* differentiatedPoly.coefficients = (double*)malloc(sizeof(double) * (myPoly.degree + 1 - 1));


differentiatedPoly.degree = myPoly.degree - 1;

for (int counter = myPoly.degree; counter > 0; counter--){

differentiatedPoly.coefficients[counter-1] = myPoly.coefficients[counter] * (counter);

}

// That loop should give out the differentiated polynomial. 




