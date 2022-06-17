/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/* Author: Dara Oseyemi                                               */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include "newton.h"
#include "horner.h"

/*--------------------------------------------------------------------*/

double guess(Polynomial_t poly, Polynomial_t polyDeriv, double convCrit) {
    double xGuess = 0;
    double oldXGuess = 0;
    
    do {
        oldXGuess = xGuess;
        xGuess -= evaluate(poly, xGuess) / evaluate(polyDeriv, xGuess);
    } while (abs(xGuess - oldXGuess) <= convCrit);

    return xGuess;
}