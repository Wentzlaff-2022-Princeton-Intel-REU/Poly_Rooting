/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/*--------------------------------------------------------------------*/

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "derivative.h"
#include "freePoly.h"
#include "longDiv.h"
#include "horner.h"
#include "newton.h"
#include "reading.h"
#define GUESS_SIZE 2

/*--------------------------------------------------------------------*/

// the compare function for double values
static int compare(const void * a, const void * b) {
  if (*(double*)a > *(double*)b)
    return 1;
  else if (*(double*)a < *(double*)b)
    return -1;
  else
    return 0;  
}

double* newton(Polynomial_t poly, double convCrit) {
    double* roots = (double*)malloc(sizeof(double) * poly.degree);
    if (roots == NULL) {
        exit(2);
    }

    for (int i = 0; i < poly.degree; i++) {
      roots[i] = DBL_MAX;
    }

    double* xGuess = (double*)malloc(sizeof(double) * GUESS_SIZE);
    double* oldXGuess = (double*)malloc(sizeof(double) * GUESS_SIZE);
    double* diff = (double*)malloc(sizeof(double) * GUESS_SIZE);
    double* oldDiff = (double*)malloc(sizeof(double) * GUESS_SIZE);

    for (int i = 0; i < GUESS_SIZE; i++){
        xGuess[i] = (double) rand() / (double) rand();
        oldXGuess[i] = 0;
        diff[i] = xGuess[i];
        oldDiff[i] = 0;
    }
    
    Polynomial_t newPoly = poly;
    Polynomial_t polyDeriv = derivative(poly);

    int i = 0;
    while (newPoly.degree > 0) {
        bool cond = true;
        bool firstLoop = true;
        do {
            bool noRoots = true;
            double* polyGuess = horner(newPoly, xGuess);
            double* polyDerivGuess = horner(polyDeriv, xGuess);
            
            for (int j = 0; j < GUESS_SIZE; j++) {
                oldXGuess[j] = xGuess[j];
                xGuess[j] -= polyGuess[j] / polyDerivGuess[j];
                oldDiff[j] = diff[j];
                diff[j] = fabs(xGuess[j] - oldXGuess[j]);
            }
            
            // printf("guess: %lf, diff: %lf\n", xGuess, fabs(xGuess - oldXGuess));

            // for (int j = 0; j < GUESS_SIZE; j++) {
            //     printf("guess: %lf, oldGuess: %lf, oldDiff: %lf, diff: %lf\n", xGuess[j], oldXGuess[j], oldDiff[j], diff[j]);
            // }

            for (int j = 0; j < GUESS_SIZE; j++) {
                noRoots = !firstLoop && diff[j] > oldDiff[j] && fabs(diff[j] - oldDiff[j]) > 1;
                if (!noRoots) {
                    break;
                }
            }
            
            if (noRoots) {
                return roots;
            }

            cond = diff[0] > convCrit && diff[1] > convCrit;
            firstLoop = false;
        } while (cond);

        for (int j = 0; j < GUESS_SIZE; j++) {
            int degree = newPoly.degree;
            newPoly = longDiv(newPoly, xGuess[j], convCrit);

            if (degree != newPoly.degree) {
                roots[i] = xGuess[j];
                i++;
            }
        }
        polyDeriv = derivative(newPoly);
    }
    freePoly(&newPoly);
    freePoly(&polyDeriv);

    qsort(roots, poly.degree, sizeof(double), compare);
    
    return roots;
}