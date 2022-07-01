/*--------------------------------------------------------------------*/
/* vecHorner.c  [incomplete]                                          */
/*--------------------------------------------------------------------*/

#include "horner.h"
#include <riscv_vector.h>



/*--------------------------------------------------------------------*/

double* vecEvaluate(Polynomial_t poly, double* x) {
  
    

    double32_t* addend1 = poly.coefficients;

    double32_t* solution;


    vfloat64m1_t va, vb, vc;



    size_t avl = poly.degree;

    for (size_t vl; (vl = vsetvl_e32m1(avl)) > 0; avl -= vl) {

       // put a vector here 
        //  every single vector element is the coefficient of the highest degree (insert a vector here)

        for (int i = poly.degree; i > 0; i--){

            va = vle32_v_f64m1(addend1, vl);
            vc = vadd_vv_f64m1(va, vb, vl);

        }

       



        vc = vadd_vv_i32m1(va, vb, vl);
        vse32_v_i32m1(sum, vc, vl);
        addend1 += vl;
        addend2 += vl;
        sum += vl;
    }



    
 // comment this snippet out when done (below)

    for(int i = poly.degree; i > 0; i--) {
        solution = poly.coefficients[i - 1] + (solution * x);
    }

    // 

    return solution;
}



