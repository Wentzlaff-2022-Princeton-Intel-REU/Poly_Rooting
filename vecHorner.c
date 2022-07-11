/*--------------------------------------------------------------------*/
/* vecHorner.c                                                        */
/*--------------------------------------------------------------------*/

#include "vecHorner.h"
#include <riscv_vector.h>


/*--------------------------------------------------------------------*/

double* vecEvaluate(Polynomial_t poly, double* x) {
  
    double* solution = (double*)malloc(sizeof(double)); 

    // declare vector registers
    vfloat64m1_t va, vb, vc;

    size_t avl = poly.degree;

    for (size_t vl; (vl = vsetvl_e32m1(avl)) > 0; avl -= vl) {

        // Filling the vector vc with the highest coefficient(s)
        vc = vfmv_v_f_f64m1(poly.coefficients[poly.degree], vl);

        // This is the vector with our guesses (x vector).
        vb = vle64_v_f64m1(x, vl); 

        for (int i = poly.degree; i > 0; i--){

            // We are moving one of the polynomial's coefficients (starting with the degree largest one - 1 and moving to the degree lowest) to a vector.
            va = vfmv_v_f_f64m1(poly.coefficients[i-1], vl); 

            // We are multiply-adding this along with the x vector (our guesses).
            vc = vfmadd_vv_f64m1(vc, vb, va, vl); 
            
        }

        // We are storing our results in the solution array.
       vse64_v_f64m1(&solution[i - avl], vc, vl);  

    } 

    return solution;
}



