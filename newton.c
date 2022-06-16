/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/* Author: Dara Oseyemi                                               */
/*--------------------------------------------------------------------*/

#include "newton.h"
#include "horner.h"

/*--------------------------------------------------------------------*/

double guess(Polynomial_t poly, Polynomial_t polyDeriv, double convCrit) {
    double xGuess = 0;

    int i;
    for (i = 0; i < poly.degree; i++) {
        double oldXGuess = xGuess;
        xGuess -= evaluate(poly, xGuess) / evaluate(polyDeriv, xGuess);

        if (abs(xGuess - oldXGuess) <= convCrit) {
            break;
        }
    }

    return xGuess;
}