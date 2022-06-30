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


static double RandomReal(double low, double high) {
  double d;

  d = (double) rand() / ((double) RAND_MAX + 1);
  return (low + d * (high - low));
}


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

    // double bigCoeff = 0;
    // for (int i = 0; i < poly.degree; i++){
    //     if (abs(poly.coefficients[i]) > abs(bigCoeff)) {
    //         bigCoeff = poly.coefficients[i];
    //     }
    // }
    // bigCoeff = abs(bigCoeff);

    // double xGuess = RandomReal((-bigCoeff-1), (bigCoeff+1));

    double* xGuess;
    double* oldXGuess;

    for (int i = 0; i < 2; i++){
        xGuess[i] = (double) rand()/ (double) rand();
        oldXGuess[i] = 0;
    }



    Polynomial_t newPoly = poly;
    Polynomial_t polyDeriv = differentiatePoly(poly);
    
    for (int i = 0; i < n; i++) {
        bool cond = true;
        do {
            for (int j = 0; j < 2; j++) {
                oldXGuess[j] = xGuess[j];
            }

            double* polyGuess = multiEvaluate(newPoly, xGuess);
            double* polyDerivGuess =  multiEvaluate(polyDeriv, xGuess);

            for (int j = 0; j < 2; j++) {
                xGuess[j] -= polyGuess[j] / polyDerivGuess[j];
            }

            // printf("guess: %lf, diff: %lf\n", xGuess, fabs(xGuess - oldXGuess));

            cond = !(fabs(xGuess[0] - oldXGuess[0]) > convCrit || fabs(xGuess[1] - oldXGuess[1]) > convCrit);

        } while (cond);
        guesses[i] = xGuess[i];

        freePoly(&polyDeriv);
        freePoly(&newPoly);

        newPoly = longDiv(newPoly, xGuess);
        polyDeriv = differentiatePoly(newPoly);
    }
    freePoly(&newPoly);
    freePoly(&polyDeriv);

    qsort(guesses, n, sizeof(double), compare);
    
    return guesses;
}