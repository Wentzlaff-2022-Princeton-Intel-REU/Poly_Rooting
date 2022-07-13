/*--------------------------------------------------------------------*/
/* vec_horner.c                                                       */
/*--------------------------------------------------------------------*/

#include <riscv_vector.h>
#include <stdio.h>
#include <stdlib.h>
#include "vec_horner.h"

/*--------------------------------------------------------------------*/

double* vec_evaluate(Polynomial_t poly, double* x, int guessSize) {
  
    double* solution = (double*)malloc(sizeof(double) * guessSize); 

    // declare vector registers
    vfloat64m1_t va, vb, vSolution;

    size_t avl = guessSize;

    for (size_t vl; (vl = vsetvl_e32m1(avl)) > 0; avl -= vl) {
        // Filling the vector vSolution with the highest coefficient(s)
        vSolution = vfmv_v_f_f64m1(poly.coefficients[poly.degree], vl);

        // This is the vector with our guesses (x vector).
        vb = vle64_v_f64m1(x, vl); 

        for (int i = poly.degree; i > 0; i--){

            // We are moving one of the polynomial's coefficients (starting with the degree largest one - 1 and moving to the degree lowest) to a vector.
            va = vfmv_v_f_f64m1(poly.coefficients[i-1], vl); 

            // We are multiply-adding this along with the x vector (our guesses).
            vSolution = vfmadd_vv_f64m1(vSolution, vb, va, vl); 
        }

        // We are storing our results in the solution array.
        vse64_v_f64m1(&solution[guessSize - avl], vSolution, vl);
    } 

    return solution;
}



