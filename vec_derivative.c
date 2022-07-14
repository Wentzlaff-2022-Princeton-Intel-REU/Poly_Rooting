/*--------------------------------------------------------------------*/
/* vec_derivative.c                                                   */
/*--------------------------------------------------------------------*/

#include <riscv_vector.h>
#include <stdlib.h>
#include "vec_derivative.h"

/*--------------------------------------------------------------------*/

/* This is a root-finding program that will take the derivative of a polynomial. 
It depends on what the structure is. */

Polynomial_t vec_derivative(Polynomial_t myPoly) {

    //create new polynomial to represent the differentiated polynomial
    Polynomial_t differentiatedPoly;
    differentiatedPoly.coefficients = (double*)malloc(sizeof(double) * (myPoly.degree + 1 - 1));  
    if(differentiatedPoly.coefficients == NULL){
        exit(2);
    }
    //initialize degree
    differentiatedPoly.degree = myPoly.degree - 1;

    //active vector length is the degree of the original polynomial
    size_t avl = myPoly.degree;

    //pointers for the resulting coefficients (after taking the derivative) and the
    //original coefficients
    double* results = differentiatedPoly.coefficients;
    double* coeffs = myPoly.coefficients + 1; //the constant at index 0 is not included in the derivative

    //array that just holds 1 to n, where n is the degree of the original polynomial.
    //this represents the exponents of the polynomial
    double* indices = (double*)malloc(sizeof(double) * (myPoly.degree + 1 - 1));
    if(indices == NULL) {
        exit(2);
    }

    for(int i = 0; i < myPoly.degree; i++){
        indices[i] = i + 1;
    }

    //declare vector registers
    vfloat64m1_t va, vb, vc;

    for (size_t vl; (vl = vsetvl_e64m1(avl)) > 0; avl -= vl) {
        //load in original coefficients into va
        va = vle64_v_f64m1(coeffs, vl);
        //load in array of exponents
        vb = vle64_v_f64m1(indices, vl);
        //multiply the two and put the result in vc
        vc = vfmul_vv_f64m1(va, vb, vl);
        //store the resulting coefficients into our differentiated polynomial
        vse64_v_f64m1(results, vc, vl);

        //move the pointers
        coeffs += vl;
        indices += vl;
        results += vl;
    }

    indices -= myPoly.degree;
    free(indices);

    return differentiatedPoly;
}
