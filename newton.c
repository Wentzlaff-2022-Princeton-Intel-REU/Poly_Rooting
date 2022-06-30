/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "newton.h"
#include "horner.h"
#include "derivative.h"
#include "reading.h"

/*--------------------------------------------------------------------*/

// performs long division on a polynomial dividend and a linear
// polynomial divisor and returns a polynomial quotient
static Polynomial_t longDiv(Polynomial_t poly, double root) {
    int n = poly.degree - 1;
    double* a_n = (double*)malloc(sizeof(double) * (n + 1));
    if (a_n == NULL) {
        exit(2);
    }

    a_n[n] = poly.coefficients[n + 1];
    for (int i = n; i > 0; i--) {
        a_n[i - 1] = poly.coefficients[i] + root * a_n[i];
    }

    Polynomial_t quotient;
    quotient.degree = n;
    quotient.coefficients = a_n;
   
    return quotient;
}

// the compare function for double values
static int compare(const void * a, const void * b) {
  if (*(double*)a > *(double*)b)
    return 1;
  else if (*(double*)a < *(double*)b)
    return -1;
  else
    return 0;  
}

double* guess(Polynomial_t poly, double convCrit) {
    int n = poly.degree;
    double* guesses = (double*)malloc(sizeof(double) * n);
    if (guesses == NULL) {
        exit(2);
    }

    double xGuess = 0.1;
    double oldXGuess = 0;

    Polynomial_t newPoly = poly;
    Polynomial_t polyDeriv = differentiatePoly(poly);
    
    for (int i = 0; i < n; i++) {
        do {
            oldXGuess = xGuess;
            xGuess -= evaluate(newPoly, xGuess) / evaluate(polyDeriv, xGuess);

            // printf("guess: %lf, diff: %lf\n", xGuess, fabs(xGuess - oldXGuess));

        } while (fabs(xGuess - oldXGuess) > convCrit);
        guesses[i] = xGuess;

        freePoly(&newPoly);
        freePoly(&polyDeriv);

        newPoly = longDiv(newPoly, xGuess);
        polyDeriv = differentiatePoly(newPoly);
    }
    freePoly(&newPoly);
    freePoly(&polyDeriv);

    
    qsort(guesses, n, sizeof(double), compare);

    freePoly(&newPoly);

    return guesses;
}