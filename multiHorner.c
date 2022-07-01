/*--------------------------------------------------------------------*/
/* horner.c                                                           */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include "multiHorner.h"

/*--------------------------------------------------------------------*/

double* multiEvaluate(Polynomial_t poly, double* x) {
    double* solution = (double*)malloc(sizeof(double) * 2);
    
    for (int i = 0; i < 2; i++) {
        solution[i] = poly.coefficients[poly.degree];
    }

    for(int i = poly.degree; i > 0; i--) {
        solution[0] = poly.coefficients[i - 1] + (solution[0] * x[0]);
        solution[1] = poly.coefficients[i - 1] + (solution[1] * x[1]);
    }

    return solution;
}
