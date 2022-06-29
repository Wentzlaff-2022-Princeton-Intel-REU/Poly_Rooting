/*--------------------------------------------------------------------*/
/* horner.c                                                           */
/*--------------------------------------------------------------------*/

#include "horner.h"

/*--------------------------------------------------------------------*/

double evaluate(Polynomial_t poly, double x) {
    double solution = poly.coefficients[poly.degree];
 
    for(int i = poly.degree; i > 0; i--) {
        solution = poly.coefficients[i - 1] + (solution * x);
    }

    return solution;
}
