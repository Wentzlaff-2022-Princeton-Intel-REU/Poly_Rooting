/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/*--------------------------------------------------------------------*/

#include <float.h>
#include <math.h>
#include <riscv_vector.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "derivative.h"
#include "freePoly.h"
#include "horner.h"
#include "longDiv.h"
#include "newton.h"

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

    double* roots = (double*)malloc(sizeof(double) * poly.degree);
    if (roots == NULL) {
        exit(2);
    }

    for (int i = 0; i < poly.degree; i++) {
      roots[i] = DBL_MAX;
    }

    size_t guessSize = vsetvlmax_e32m1();
    // if (poly.degree < 8) {
    //     guessSize = poly.degree;
    // }

    double* guesses = (double*)malloc(sizeof(double) * guessSize);
    // double* oldXGuess = (double*)malloc(sizeof(double) * guessSize);
    // double* diff = (double*)malloc(sizeof(double) * guessSize);
    // double* oldDiff = (double*)malloc(sizeof(double) * guessSize);

    vfloat64m1_t vGuesses, vOldGuesses, vDiff, vOldDiff;

    // for (int i = 0; i < guessSize; i++){
    //     xGuess[i] = 0;
    // //     oldXGuess[i] = 0;
    // //     diff[i] = xGuess[i];
    // //     oldDiff[i] = 0;
    // }

    vGuesses = vfcvt_f_xu_v_f64m1(vid_v_u64m1(guessSize), guessSize);
    vOldGuesses = vfmv_v_f_f64m1(0, guessSize);
    vDiff = vGuesses;
    vOldDiff = vOldGuesses;

    // Polynomial_t newPoly = poly;
    Polynomial_t polyDeriv = derivative(poly);

    int i = 0;
    while (poly.degree > 0) {
        // bool cond = true;
        long cond1 = 0;
        bool firstLoop = true;
        do {
            // bool noRoots = true;
            // double* polyGuess = horner(poly, xGuess, guessSize);
            // double* polyDerivGuess = horner(polyDeriv, xGuess, guessSize);

            vfloat64m1_t polyGuess, polyDerivGuess;
            polyGuess = horner(poly, vGuesses, guessSize);
            polyDerivGuess = horner(polyDeriv, vGuesses, guessSize);

            // vfloat64m1_t ve, vf;
            // ones = vfmv_v_f_f64m1(1, guessSize);
            // ve = vle64_v_f64m1(polyGuess, guessSize);
            // vf = vle64_v_f64m1(polyDerivGuess, guessSize);
            // vf = vfdiv_vv_f64m1(ones, vf, guessSize);

            // for (int j = 0; j < 2; j++) {
            //     oldXGuess[j] = xGuess[j];
            //     xGuess[j] -= polyGuess[j] / polyDerivGuess[j];
            //     oldDiff[j] = diff[j];
            //     diff[j] = fabs(xGuess[j] - oldXGuess[j]);
            // }

            vOldGuesses = vmv_v_v_f64m1(vGuesses, guessSize);
            // va = vfnmsac_vv_f64m1(va, ve, vf, guessSize);
            vGuesses = vfsub_vv_f64m1(vGuesses, vfdiv_vv_f64m1(polyGuess, polyDerivGuess, guessSize), guessSize);
            vOldDiff = vmv_v_v_f64m1(vDiff, guessSize);
            vDiff = vfabs_v_f64m1(vfsub_vv_f64m1(vGuesses, vOldGuesses, guessSize), guessSize);

            // printf("guess: %lf, diff: %lf\n", xGuess, fabs(xGuess - oldXGuess));

            // for (int j = 0; j < 2; j++) {
            //     printf("guess: %lf, oldGuess: %lf, oldDiff: %lf, diff: %lf\n", xGuess[j], oldXGuess[j], oldDiff[j], diff[j]);
            // }

            // for (int j = 0; j < 2; j++) {
            //     noRoots = !firstLoop && diff[j] > oldDiff[j] && fabs(diff[j] - oldDiff[j]) > 1;
            //     if (!noRoots) {
            //         break;
            //     }
            // }

            vbool64_t greaterDiff, greaterThan1, greaterThanConvCrit;

            greaterDiff = vmfgt_vv_f64m1_b64(vDiff, vOldDiff, guessSize);
            greaterThan1 = vmfgt_vf_f64m1_b64(vfabs_v_f64m1(vfsub_vv_f64m1(vDiff, vOldDiff, guessSize), guessSize), 1, guessSize);
            // vb3 = vmand_mm_b64(vb1, vb2, guessSize);
            // long noRoots1 = vfirst_m_b64(vmnot_m_b64(vb3, guessSize), guessSize);
            long noMoreRoots = vfirst_m_b64(vmnand_mm_b64(greaterDiff, greaterThan1, guessSize), guessSize);

            if (!firstLoop && noMoreRoots == -1) {
                qsort(roots, n, sizeof(double), compare);
                return roots;
            }

            // cond = diff[0] > convCrit && diff[1] > convCrit;

            vfloat64m1_t crit;
            crit = vfmv_v_f_f64m1(convCrit, guessSize);
            greaterThanConvCrit = vmfgt_vv_f64m1_b64(vDiff, crit, guessSize);
            // cond1 = vfirst_m_b64(vmnot_m_b64(vb4, guessSize), guessSize);
            if (vfirst_m_b64(vmnot_m_b64(greaterThanConvCrit, guessSize), guessSize) != -1) {
                break;
            }

            firstLoop = false;
        } while (true);

        vse64_v_f64m1(guesses, vGuesses, guessSize);
        for (int j = 0; j < guessSize; j++) {
            // equivalent to second condition on line 27 in main.c
            // if (isnan(guesses[j])) {
            //     qsort(roots, n, sizeof(double), compare);
            //     return roots;
            // }

            int degree = poly.degree;
            poly = longDiv(poly, guesses[j], convCrit);

            if (degree != poly.degree) {
                roots[i] = guesses[j];
                i++;
            }
        }
        polyDeriv = derivative(poly);
    }
    free(guesses);
    freePoly(&poly);
    freePoly(&polyDeriv);

    qsort(roots, n, sizeof(double), compare);
    
    return roots;
}