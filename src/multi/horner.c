/*--------------------------------------------------------------------*/
/* horner.c                                                           */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include "horner.h"
#define GUESS_SIZE 2

/*--------------------------------------------------------------------*/

double* horner(Polynomial_t poly, double* guesses) {
    double* solution = (double*)malloc(sizeof(double) * GUESS_SIZE);
    for (int i = 0; i < GUESS_SIZE; i++) {
        solution[i] = poly.coefficients[poly.degree];
    }

    for(int i = poly.degree; i > 0; i--) {
        solution[0] = poly.coefficients[i - 1] + (solution[0] * guesses[0]);
        solution[1] = poly.coefficients[i - 1] + (solution[1] * guesses[1]);
    }

    return solution;
}
