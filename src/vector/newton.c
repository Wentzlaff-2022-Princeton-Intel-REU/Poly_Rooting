/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/*--------------------------------------------------------------------*/

#include <float.h>
#include <math.h>
#include <riscv_vector.h>
#include <stdbool.h>
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
    double* guesses = (double*)malloc(sizeof(double) * guessSize);

    vfloat64m1_t vGuesses, vOldGuesses, vDiff, vOldDiff;
    vGuesses = vfcvt_f_xu_v_f64m1(vid_v_u64m1(guessSize), guessSize);
    vOldGuesses = vfmv_v_f_f64m1(0, guessSize);
    vDiff = vGuesses;
    vOldDiff = vOldGuesses;

    Polynomial_t polyDeriv = derivative(poly);

    int rootIndex = 0;
    while (poly.degree > 0) {
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

            vOldGuesses = vGuesses;
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

            vbool64_t greaterDiff, greaterThan1;
            greaterDiff = vmfgt_vv_f64m1_b64(vDiff, vOldDiff, guessSize);
            greaterThan1 = vmfgt_vf_f64m1_b64(vfabs_v_f64m1(vfsub_vv_f64m1(vDiff, vOldDiff, guessSize), guessSize), 1, guessSize);
            long noMoreRoots = vfirst_m_b64(vmnand_mm_b64(greaterDiff, greaterThan1, guessSize), guessSize);

            if (poly.degree % 2 == 0 && !firstLoop && noMoreRoots == -1) {
                free(guesses);
                freePoly(&poly);
                freePoly(&polyDeriv);

                qsort(roots, rootIndex + 1, sizeof(double), compare);
                return roots;
            }

            // cond = diff[0] > convCrit && diff[1] > convCrit;

            long diffGreaterThanConvCrit = vfirst_m_b64(vmnot_m_b64(vmfgt_vf_f64m1_b64(vDiff, convCrit, guessSize), guessSize), guessSize);
            long oldDiffGreaterThanConvCrit = vfirst_m_b64(vmnot_m_b64(vmfgt_vf_f64m1_b64(vOldDiff, convCrit, guessSize), guessSize), guessSize);
            // long diffGreaterThanConvCrit = vfirst_m_b64(vmfle_vf_f64m1_b64(vDiff, convCrit, guessSize), guessSize);
            // long oldDiffGreaterThanConvCrit = vfirst_m_b64(vmfle_vf_f64m1_b64(vOldDiff, convCrit, guessSize), guessSize);

            // cond1 = vfirst_m_b64(vmnot_m_b64(vb4, guessSize), guessSize);
            if (diffGreaterThanConvCrit != -1 && oldDiffGreaterThanConvCrit != -1) {
                break;
            }

            firstLoop = false;
        } while (true);

        vse64_v_f64m1(guesses, vGuesses, guessSize);
        bool notFinite = false;
        for (int j = 0; j < guessSize; j++) {
            int degree = poly.degree;
            poly = longDiv(poly, guesses[j], convCrit);

            if (degree != poly.degree) {
                roots[rootIndex] = guesses[j];
                rootIndex++;
            }
            else if (!isfinite(guesses[j])) {
                notFinite = true;
                guesses[j] = (rand() % 11) - 5;
            }
        }

        polyDeriv = derivative(poly);
        if (notFinite) {
            vGuesses = vle64_v_f64m1(guesses, guessSize);
        }
    }
    free(guesses);
    freePoly(&poly);
    freePoly(&polyDeriv);

    qsort(roots, n, sizeof(double), compare);
    return roots;
}