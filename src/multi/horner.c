/*--------------------------------------------------------------------*/
/* horner.c                                                           */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include "horner.h"
#define GUESS_SIZE 2

/*--------------------------------------------------------------------*/

double* horner(Polynomial_t poly, double* guesses) {
    double* solution = (double*)malloc(sizeof(double) * GUESS_SIZE);
    
    for (int coeffIterator = 0; coeffIterator < GUESS_SIZE; coeffIterator++) {
        solution[coeffIterator] = poly.coefficients[poly.degree];
    }

    for(int solIterator = poly.degree; solIterator > 0; solIterator--) {
        solution[solIterator] = poly.coefficients[solIterator - 1] + (solution[0] * guesses[0]);
        solution[solIterator] = poly.coefficients[solIterator - 1] + (solution[1] * guesses[1]);
    }

    return solution;
}
