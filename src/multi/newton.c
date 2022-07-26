/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/*--------------------------------------------------------------------*/

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "derivative.h"
#include "freePoly.h"
#include "horner.h"
#include "longDiv.h"
#include "newton.h"
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
    int n = poly.degree;
    double* roots = (double*)malloc(sizeof(double) * n);
    if (roots == NULL) {
        exit(2);
    }

    for (int i = 0; i < n; i++) {
      roots[i] = DBL_MAX;
    }

    double* guesses = (double*)malloc(sizeof(double) * GUESS_SIZE);
    double* oldGuesses = (double*)malloc(sizeof(double) * GUESS_SIZE);
    double* diff = (double*)malloc(sizeof(double) * GUESS_SIZE);
    double* oldDiff = (double*)malloc(sizeof(double) * GUESS_SIZE);

    for (int i = 0; i < GUESS_SIZE; i++){
        /* guesses is intialized to some random integer in range [-5, 5]
        guesses[i] = (rand() % (upper - lower + 1)) + lower
        where upper and lower equal 5 and -5, respectively */ 
        guesses[i] = (rand() % 11) - 5;
        oldGuesses[i] = 0;
        diff[i] = guesses[i];
        oldDiff[i] = 0;
    }
    
    Polynomial_t polyDeriv = derivative(poly);

    int rootIndex = 0;
    while (poly.degree > 0) {
        bool firstLoop = true;
        do {
            bool noRoots = true;
            double* polyGuess = horner(poly, guesses);
            double* polyDerivGuess = horner(polyDeriv, guesses);
            
            for (int j = 0; j < GUESS_SIZE; j++) {
                oldGuesses[j] = guesses[j];
                guesses[j] -= polyGuess[j] / polyDerivGuess[j];
                oldDiff[j] = diff[j];
                diff[j] = fabs(guesses[j] - oldGuesses[j]);
            }

            for (int j = 0; j < GUESS_SIZE; j++) {
                noRoots = !firstLoop && diff[j] > oldDiff[j] && fabs(diff[j] - oldDiff[j]) > 1;
                if (!noRoots) {
                    break;
                }
            }
            
            if (poly.degree % 2 == 0 && (noRoots)) {
                free(guesses);
                free(oldGuesses);
                free(diff);
                free(oldDiff);
                freePoly(&poly);
                freePoly(&polyDeriv);

                qsort(roots, rootIndex + 1, sizeof(double), compare);
                return roots;
            }

            // (diff[0] <= convCrit && oldDiff[0] <= convCrit) || (diff[1] <= convCrit && oldDiff[1] <= convCrit)
            if (!((diff[0] > convCrit && diff[1] > convCrit) || (oldDiff[0] > convCrit && oldDiff[1] > convCrit))) {
                break;
            }

            firstLoop = false;
        } while (true);

        for (int j = 0; j < GUESS_SIZE; j++) {
            int degree = poly.degree;
            poly = longDiv(poly, guesses[j], convCrit);

            if (degree != poly.degree) {
                roots[rootIndex] = guesses[j];
                rootIndex++;
            }
            else if (!isfinite(guesses[j])) {
                guesses[j] = (rand() % 11) - 5;
            }
        }
        polyDeriv = derivative(poly);
    }
    free(guesses);
    free(oldGuesses);
    free(diff);
    free(oldDiff);
    freePoly(&poly);
    freePoly(&polyDeriv);

    qsort(roots, n, sizeof(double), compare);
    return roots;
}