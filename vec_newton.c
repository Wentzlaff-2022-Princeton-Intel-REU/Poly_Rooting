/*--------------------------------------------------------------------*/
/* vec_newton.c                                                       */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <riscv_vector.h>
#include "vec_newton.h"
#include "vecHorner.h"
#include "vec_derivative.h"
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

double* vec_guess(Polynomial_t poly, double convCrit) {
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
    vb = vfmv_v_f_f64m1(0, guessSize);
    vc = vle64_v_f64m1(xGuess, guessSize);
    vd = vfmv_v_f_f64m1(0, guessSize);

    Polynomial_t newPoly = poly;
    Polynomial_t polyDeriv = vec_differentiatePoly(poly);

    int i = 0;
    while (newPoly.degree > 0) {
        // bool cond = true;
        long cond1 = 0;
        bool firstLoop = true;
        do {
            // bool noRoots = true;
            double* polyGuess = vecEvaluate(newPoly, xGuess, guessSize);
            double* polyDerivGuess = vecEvaluate(polyDeriv, xGuess, guessSize);

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
            newPoly = longDiv(newPoly, xGuess[j]);

            if (degree != newPoly.degree) {
                roots[i] = xGuess[j];
                i++;
            }
        }
        polyDeriv = vec_differentiatePoly(newPoly);
        printf("degree: %d\n", newPoly.degree);
    }
    //freePoly(&newPoly);
    //freePoly(&polyDeriv);

    qsort(roots, poly.degree, sizeof(double), compare);
    
    return roots;
}