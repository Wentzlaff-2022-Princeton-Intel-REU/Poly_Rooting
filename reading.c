/*--------------------------------------------------------------------*/
/* reading.c                                                          */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "reading.h"

/*--------------------------------------------------------------------*/

Polynomial_t readPoly(void) {
    int n = 0;
    scanf("%d", &n);

    double* a_n = (double*)malloc(sizeof(double) * (n + 1));
    if (a_n == NULL) {
        exit(2);
    }

    for (int i = 0; i <= n; i++) {
        scanf("%lf", &a_n[n - i]);
    }

    Polynomial_t poly;
    poly.degree = n;
    poly.coefficients = a_n;

    return poly;
}