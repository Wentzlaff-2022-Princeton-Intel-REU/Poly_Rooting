/*--------------------------------------------------------------------*/
/* newton.c                                                           */
/*--------------------------------------------------------------------*/

#include <float.h>
#include <math.h>
#include <riscv_vector.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "freePoly.h"
#include "longDiv.h"
#include "derivative.h"
#include "horner.h"
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
    double* roots [poly.degree];

    for (int i = 0; i < poly.degree; i++) {
      roots[i] = DBL_MAX;
    }

    int guessSize = 8;
    if (poly.degree < 8) {
        guessSize = poly.degree;
    }

    double* xGuess [guessSize];
    // double* oldXGuess [guessSize];
    // double* diff [guessSize];
    // double* oldDiff [guessSize];

    vfloat64m1_t va, vb, vc, vd;

    for (int i = 0; i < guessSize; i++){
        xGuess[i] = (double) rand() / (double) rand();
        // oldXGuess[i] = 0;
        // diff[i] = xGuess[i];
        // oldDiff[i] = 0;
    }

    va = vle64_v_f64m1(xGuess, guessSize);
    vb = vfmv_v_f_f64m1(0, guessSize);
    vc = vle64_v_f64m1(xGuess, guessSize);
    vd = vfmv_v_f_f64m1(0, guessSize);

    Polynomial_t newPoly = poly;
    Polynomial_t polyDeriv = derivative(poly);

    int i = 0;
    while (newPoly.degree > 0) {
        // bool cond = true;
        long cond1 = 0;
        bool firstLoop = true;
        do {
            // bool noRoots = true;
            double* polyGuess = horner(newPoly, xGuess, guessSize);
            double* polyDerivGuess = horner(polyDeriv, xGuess, guessSize);

            vfloat64m1_t ve, vf, ones;
            ones = vfmv_v_f_f64m1(1, guessSize);
            ve = vle64_v_f64m1(polyGuess, guessSize);
            vf = vle64_v_f64m1(polyDerivGuess, guessSize);
            vf = vfdiv_vv_f64m1(ones, vf, guessSize);

            // for (int j = 0; j < 2; j++) {
            //     oldXGuess[j] = xGuess[j];
            //     xGuess[j] -= polyGuess[j] / polyDerivGuess[j];
            //     oldDiff[j] = diff[j];
            //     diff[j] = fabs(xGuess[j] - oldXGuess[j]);
            // }

            vb = vmv_v_v_f64m1(va, guessSize);
            va = vfnmsac_vv_f64m1(va, ve, vf, guessSize);
            vd = vmv_v_v_f64m1(vc, guessSize);
            vc = vfabs_v_f64m1(vfsub_vv_f64m1(va, vb, guessSize), guessSize);

            vse64_v_f64m1(xGuess, va, guessSize);  

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

            vbool64_t vb1, vb2, vb3, vb4;

            vb1 = vmfgt_vv_f64m1_b64(vc, vd, guessSize);
            vb2 = vmfgt_vf_f64m1_b64(vfabs_v_f64m1(vfsub_vv_f64m1(vc, vd, guessSize), guessSize), 1, guessSize);
            vb3 = vmand_mm_b64(vb1, vb2, guessSize);
            long noRoots1 = vfirst_m_b64(vmnot_m_b64(vb3, guessSize), guessSize);

            if (!firstLoop && noRoots1 == -1) {
                return roots;
            }

            // cond = diff[0] > convCrit && diff[1] > convCrit;

            vfloat64m1_t crit;
            crit = vfmv_v_f_f64m1(convCrit, guessSize);
            vb4 = vmfgt_vv_f64m1_b64(vc, crit, guessSize);
            cond1 = vfirst_m_b64(vmnot_m_b64(vb4, guessSize), guessSize);

            firstLoop = false;
        } while (cond1 == -1);
        //freePoly(&newPoly);
        //freePoly(&polyDeriv);

        for (int j = 0; j < guessSize; j++) {
            int degree = newPoly.degree;
            newPoly = longDiv(newPoly, xGuess[j], convCrit);

            if (degree != newPoly.degree) {
                roots[i] = xGuess[j];
                i++;
            }
        }
        polyDeriv = derivative(newPoly);
    }
    free(xGuess);
    freePoly(&newPoly);
    freePoly(&polyDeriv);

    qsort(roots, poly.degree, sizeof(double), compare);
    
    return roots;
}