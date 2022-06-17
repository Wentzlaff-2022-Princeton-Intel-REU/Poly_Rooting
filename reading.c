/*--------------------------------------------------------------------*/
/* reading.c                                                          */
/* Author: Dara Oseyemi                                               */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "reading.h"

/*--------------------------------------------------------------------*/

Polynomial_t readPoly(void) {
    Polynomial_t poly;
    int n = 0;
    scanf("%d", &n);

    int i;
    double* a_n = (double*)malloc(sizeof(double) * (n + 1));
    if (a_n == NULL) {
        exit(2);
    }

    for (i = 0; i <= n; i++) {
        scanf("%lf", &a_n[n - i]);
    }

    poly.degree = n;
    poly.coefficients = a_n;

    return poly;
}