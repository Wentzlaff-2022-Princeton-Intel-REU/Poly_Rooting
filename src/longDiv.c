/*--------------------------------------------------------------------*/
/* longDiv.c                                                          */
/*--------------------------------------------------------------------*/

#include <math.h>
#include <stdlib.h>
#include "longDiv.h"

/*--------------------------------------------------------------------*/

Polynomial_t longDiv(Polynomial_t poly, double root, double diff) {
    int n = poly.degree - 1;
    double* a_n = (double*)malloc(sizeof(double) * (n + 1));
    if (a_n == NULL) {
        exit(2);
    }

    a_n[n] = poly.coefficients[n + 1];
    for (int i = n; i > 0; i--) {
        a_n[i - 1] = poly.coefficients[i] + root * a_n[i];
    }

    // printf("root: %.16lf, diff: %.16lf\n", root, (poly.coefficients[0] + root * a_n[0]));
    if (!isfinite(root) || fabs(poly.coefficients[0] + root * a_n[0]) > diff) {
        return poly;
    }

    Polynomial_t quotient;
    quotient.degree = n;
    quotient.coefficients = a_n;
   
    return quotient;
}