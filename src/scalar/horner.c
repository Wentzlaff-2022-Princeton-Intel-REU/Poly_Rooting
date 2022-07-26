/*--------------------------------------------------------------------*/
/* horner.c                                                           */
/*--------------------------------------------------------------------*/

#include "horner.h"

/*--------------------------------------------------------------------*/

double horner(Polynomial_t poly, double guesses) {
    double solution = poly.coefficients[poly.degree];

    for (int i = poly.degree; i > 0; i--) {
        solution = poly.coefficients[i - 1] + (solution * guesses);
    }

    return solution;
}
