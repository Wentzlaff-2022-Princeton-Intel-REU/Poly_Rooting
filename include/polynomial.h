#pragma once
/*------------------------------------------------------------*/
/* polynomial.h                                               */
/*------------------------------------------------------------*/

/* A Polynomial is a struct represented by a degree and
an array of coefficients (index 0 holds the coefficient of the 
lowest degree) */
typedef struct Polynomial {
    int degree;
    double* coefficients;
} Polynomial_t;
