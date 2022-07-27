/*--------------------------------------------------------------------*/
/* horner.c                                                           */
/*--------------------------------------------------------------------*/

#include <riscv_vector.h>
#include "horner.h"

/*--------------------------------------------------------------------*/

vfloat64m1_t horner(Polynomial_t poly, vfloat64m1_t vGuesses, size_t guessSize) {
    vfloat64m1_t currCoeff, solutions;

    for (size_t vl; (vl = vsetvl_e32m1(guessSize)) > 0; guessSize -= vl) {
        // filling the vector solutions with the coefficient of the high degree
        solutions = vfmv_v_f_f64m1(poly.coefficients[poly.degree], vl);

        for (int i = poly.degree; i > 0; i--){
            /* moves g one of the polynomial's coefficients
            (starting with the coefficient of the second high degree and
            moving to that of the lowest degree) to a vector */
            currCoeff = vfmv_v_f_f64m1(poly.coefficients[i-1], vl); 
            // solutions += vGuesses * currCoeff
            solutions = vfmadd_vv_f64m1(solutions, vGuesses, currCoeff, vl); 
        }
    }
    return solutions;
}
