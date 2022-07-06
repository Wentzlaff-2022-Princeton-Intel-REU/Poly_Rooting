/*--------------------------------------------------------------------*/
/* vec_newton.c                                                           */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <riscv_vector.h>
#include "vec_newton.h"
#include "vec_horner.h"
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

    double min = 1e-14;
    a_n[n] = poly.coefficients[n + 1];
    for (int i = n; i > 0; i--) {
        a_n[i - 1] = poly.coefficients[i] + root * a_n[i];
    }

    printf("root: %.16lf, diff: %.16lf\n", root, (poly.coefficients[0] + root * a_n[0]));
    if (fabs(poly.coefficients[0] + root * a_n[0]) > min) {
        return poly;
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

double* multiGuess(Polynomial_t poly, double convCrit) {
    double* roots = (double*)malloc(sizeof(double) * poly.degree);
    if (roots == NULL) {
        exit(2);
    }

    for (int i = 0; i < poly.degree; i++) {
      roots[i] = DBL_MAX;
    }

    int guessSize = 8;
    if (poly.degree < 8) {
        guessSize = poly.degree;
    }

    double* xGuess = (double*)malloc(sizeof(double) * guessSize);
    // double* oldXGuess = (double*)malloc(sizeof(double) * guessSize);
    // double* diff = (double*)malloc(sizeof(double) * guessSize);
    // double* oldDiff = (double*)malloc(sizeof(double) * guessSize);

    vfloat64m1_t va, vb, vc, vd;

    
    for (int i = 0; i < guessSize; i++){
        xGuess[i] = (double) rand() / (double) rand();
        // oldXGuess[i] = 0;
        // diff[i] = xGuess[i];
        // oldDiff[i] = 0;
    }

    va = vle64_v_f64m1(xGuess, guessSize);
    vb = vfmv_v_f_f64m(0, guessSize);
    vc = vle64_v_f64m1(xGuess, guessSize);
    vd = vfmv_v_f_f64m(0, guessSize);

    // printf("2\n");
    Polynomial_t newPoly = poly;
    Polynomial_t polyDeriv = differentiatePoly(poly);

    int i = 0;
    while (newPoly.degree > 0) {
        bool cond = true;
        bool firstLoop = true;
        do {
            // printf("3\n");
            bool noRoots = true;
            double* polyGuess = multiEvaluate(newPoly, xGuess);
            double* polyDerivGuess = multiEvaluate(polyDeriv, xGuess);

            vfloat64m1_t ve, vf, ones;
            ones = vfmv_v_f_f64m(0, guessSize);
            ve = vle64_v_f64m1(polyGuess, guessSize);
            vf = vle64_v_f64m1(polyDerivGuess, guessSize);
            vf = vfdiv_vv_f64m1(ones, vf, guessSize);

            // printf("4\n");
            // for (int j = 0; j < 2; j++) {
            //     oldXGuess[j] = xGuess[j];
            //     xGuess[j] -= polyGuess[j] / polyDerivGuess[j];
            //     oldDiff[j] = diff[j];
            //     diff[j] = fabs(xGuess[j] - oldXGuess[j]);
            // }

            vb = vmv_v_v_f64m1(va, guessSize);
            va = vfnmsac_vv_f64m1(va, ve, vf, guessSize);
            vd = vmv_v_v_f64m1(vc, guessSize);
            vc = vfsub_vv_f64m1(va, vb, guessSize);
            vc = vfabs_v_f64m1(vc, guessSize);

            vse64_v_f64m1(&xGuess, va, guessSize);  

            // printf("5\n");
            // printf("guess: %lf, diff: %lf\n", xGuess, fabs(xGuess - oldXGuess));

            // for (int j = 0; j < 2; j++) {
            //     printf("guess: %lf, oldGuess: %lf, oldDiff: %lf, diff: %lf\n", xGuess[j], oldXGuess[j], oldDiff[j], diff[j]);
            // }

            for (int j = 0; j < 2; j++) {
                noRoots = !firstLoop && diff[j] > oldDiff[j] && fabs(diff[j] - oldDiff[j]) > 1;
                if (!noRoots) {
                    break;
                }
            }

            // vmfgt_vv_f64m1_b64
            // vmfgt_vf_f64m1_b64

            // printf("6\n");
            if (noRoots) {
                return roots;
            }

            cond = diff[0] > convCrit && diff[1] > convCrit;
            firstLoop = false;
        } while (cond);
        // roots[i] = xGuess[i];
        // printf("7\n");
        freePoly(&newPoly);
        freePoly(&polyDeriv);

        for (int j = 0; j < 2; j++) {
            int degree = newPoly.degree;
            newPoly = longDiv(newPoly, xGuess[j]);

            if (degree != newPoly.degree) {
                roots[i] = xGuess[j];
                i++;
            }
        }
        polyDeriv = differentiatePoly(newPoly);
    }
    freePoly(&newPoly);
    freePoly(&polyDeriv);

    qsort(roots, poly.degree, sizeof(double), compare);
    
    return roots;
}