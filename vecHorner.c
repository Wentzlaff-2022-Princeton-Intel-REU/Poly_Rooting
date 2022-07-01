/*--------------------------------------------------------------------*/
/* vecHorner.c                                                           */
/*--------------------------------------------------------------------*/

#include "horner.h"
#include <riscv_vector.h>



/*--------------------------------------------------------------------*/

double vecEvaluate(Polynomial_t poly, double x) {
    double solution = poly.coefficients[poly.degree];
    
    vint32m1_t vb, vc;


    for (size_t vl; (vl = vsetvl_e32m1(avl)) > 0; avl -= vl) {
        va = vle32_v_i32m1(addend1, vl);
        vb = vle32_v_i32m1(addend2, vl);
        vc = vadd_vv_i32m1(va, vb, vl);
        vse32_v_i32m1(sum, vc, vl);
        addend1 += vl;
        addend2 += vl;
        sum += vl;
    }



    
 
    for(int i = poly.degree; i > 0; i--) {
        solution = poly.coefficients[i - 1] + (solution * x);
    }

    return solution;
}
