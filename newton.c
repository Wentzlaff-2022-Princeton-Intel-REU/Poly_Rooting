/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/* Author: Dara Oseyemi                                               */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include "newton.h"
#include "horner.h"

/*--------------------------------------------------------------------*/

double guess(Polynomial_t poly, Polynomial_t polyDeriv, double convCrit) {
    double xGuess = 0;
    double oldXGuess = 0;
    
    do {
        oldXGuess = xGuess;
        xGuess -= evaluate(poly, xGuess) / evaluate(polyDeriv, xGuess);

        printf("guess: %lf, diff: %lf\n", xGuess, fabs(xGuess - oldXGuess));

    } while (fabs(xGuess - oldXGuess) > convCrit);

    return xGuess;
}